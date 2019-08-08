#
# Copyright (c) 2013 Pavlo Lavrenenko
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

cmake_minimum_required (VERSION 3.12)
project (GRAPHENE)

find_package (OpenGL REQUIRED)
find_package (Freetype REQUIRED)

if (UNIX)
    find_package(X11 REQUIRED)
endif ()

find_path (MATH_INCLUDE_DIRS NAMES MathApi.h HINTS ENV MATH_DIR PATH_SUFFIXES include)
find_library (MATH_LIBRARIES NAMES math math-shared HINTS ENV MATH_DIR PATH_SUFFIXES lib)
if (NOT MATH_INCLUDE_DIRS OR NOT MATH_LIBRARIES)
    message (FATAL_ERROR "Could NOT find Math")
endif ()

find_path (SIGNALS_INCLUDE_DIRS NAMES Signals.h HINTS ENV SIGNALS_DIR PATH_SUFFIXES include)
if (NOT SIGNALS_INCLUDE_DIRS)
    message (FATAL_ERROR "Could NOT find Signals")
endif ()

set (GRAPHENE_NAME Graphene)
set (GRAPHENE_DESCRIPTION "OpenGL-based Graphics Engine")
set (GRAPHENE_VERSION 0.2.0)

set (GRAPHENE_LIBRARY graphene)
set (GRAPHENE_STATIC ${GRAPHENE_LIBRARY}-static)
set (GRAPHENE_SHARED ${GRAPHENE_LIBRARY}-shared)

set (GRAPHENE_DOCUMENTATION OFF CACHE BOOL "Build documentation")
set (GRAPHENE_EXAMPLE OFF CACHE BOOL "Build example")
set (GRAPHENE_TEST OFF CACHE BOOL "Build unit tests")

if (GRAPHENE_DOCUMENTATION)
    find_package (Doxygen REQUIRED dot)
    add_custom_target (${GRAPHENE_LIBRARY}-docs ALL ${DOXYGEN_EXECUTABLE})
endif ()

if (GRAPHENE_EXAMPLE)
    add_subdirectory (example)
endif ()

if (GRAPHENE_TEST)
    enable_testing ()
    add_subdirectory (test)
endif ()

file (GLOB_RECURSE GRAPHENE_SOURCES src/*.cpp)
file (GLOB_RECURSE GRAPHENE_HEADERS src/*.h)
include_directories (src ${OPENGL_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS} ${MATH_INCLUDE_DIRS} ${SIGNALS_INCLUDE_DIRS})

if (UNIX)
    include_directories (${X11_INCLUDE_DIRS})
endif ()

add_library (${GRAPHENE_LIBRARY} OBJECT ${GRAPHENE_SOURCES})
set_target_properties (${GRAPHENE_LIBRARY} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE YES
)
target_compile_options (${GRAPHENE_LIBRARY} PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror -pedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
)
target_compile_definitions (${GRAPHENE_LIBRARY} PUBLIC GRAPHENE_EXPORT UNICODE _USE_MATH_DEFINES)

add_library (${GRAPHENE_STATIC} STATIC $<TARGET_OBJECTS:${GRAPHENE_LIBRARY}>)
add_library (${GRAPHENE_SHARED} SHARED $<TARGET_OBJECTS:${GRAPHENE_LIBRARY}>)
set_target_properties (${GRAPHENE_SHARED} PROPERTIES VERSION ${GRAPHENE_VERSION} SOVERSION ${GRAPHENE_VERSION})

if (WIN32)
    # Dedicated library names for Win32 platform (dynamic library target outputs .lib as well)
    set_target_properties (${GRAPHENE_STATIC} PROPERTIES OUTPUT_NAME ${GRAPHENE_STATIC})
    set_target_properties (${GRAPHENE_SHARED} PROPERTIES OUTPUT_NAME ${GRAPHENE_SHARED})
elseif (UNIX)
    set_target_properties (${GRAPHENE_STATIC} ${GRAPHENE_SHARED} PROPERTIES OUTPUT_NAME ${GRAPHENE_LIBRARY})
endif ()

set (GRAPHENE_LINK_LIBRARIES ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} ${MATH_LIBRARIES})
target_link_libraries (${GRAPHENE_STATIC} ${GRAPHENE_LINK_LIBRARIES})
target_link_libraries (${GRAPHENE_SHARED} ${GRAPHENE_LINK_LIBRARIES})

if (UNIX)
    target_link_libraries (${GRAPHENE_STATIC} ${X11_LIBRARIES})
    target_link_libraries (${GRAPHENE_SHARED} ${X11_LIBRARIES})
endif ()

install (TARGETS ${GRAPHENE_STATIC} ${GRAPHENE_SHARED} DESTINATION lib)
install (FILES ${GRAPHENE_HEADERS} DESTINATION include/${GRAPHENE_LIBRARY})

if (UNIX)
    configure_file (graphene.pc.in graphene.pc @ONLY)
    install (FILES ${PROJECT_BINARY_DIR}/graphene.pc DESTINATION lib/pkgconfig)
endif ()

if (DOXYGEN_FOUND)
    configure_file (doc/Doxyfile.in Doxyfile @ONLY)
    install (DIRECTORY ${PROJECT_BINARY_DIR}/html
             DESTINATION $<IF:$<BOOL:UNIX>,share/doc/${GRAPHENE_LIBRARY}-${GRAPHENE_VERSION},docs>)
endif ()
