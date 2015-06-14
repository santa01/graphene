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

cmake_minimum_required (VERSION 2.8.9)
project (GRAPHENE)

if (CMAKE_COMPILER_IS_GNUCC)
    execute_process (COMMAND ${CMAKE_CXX_COMPILER} -dumpversion
                     OUTPUT_VARIABLE GCC_VERSION)
    string (STRIP ${GCC_VERSION} GCC_VERSION)
endif ()

if (DEFINED GCC_VERSION AND GCC_VERSION VERSION_GREATER 4.7.0)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=gnu++11")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g -O3")
else ()
    message (FATAL_ERROR "GCC 4.7.0 or higher is required!")
endif ()

find_package (OpenGL REQUIRED)
find_package (GLEW REQUIRED)

include (FindPkgConfig)
pkg_search_module (SDL2 REQUIRED sdl2)
pkg_search_module (SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module (MATH REQUIRED math)

include (CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX (Signals.h HAVE_SIGNALS_H)
if (NOT HAVE_SIGNALS_H)
    message (FATAL_ERROR "Header Signals.h not found!")
endif ()

file (GLOB_RECURSE GRAPHENE_SOURCES src/*.cpp)
file (GLOB_RECURSE GRAPHENE_HEADERS src/*.h)
include_directories (src ${MATH_INCLUDE_DIRS})

set (GRAPHENE_NAME Graphene)
set (GRAPHENE_DESCRIPTION "OpenGL-based Graphics Engine")
set (GRAPHENE_VERSION 0.1.5)

set (GRAPHENE_LIBRARY graphene)
set (GRAPHENE_STATIC ${GRAPHENE_LIBRARY}-static)
set (GRAPHENE_SHARED ${GRAPHENE_LIBRARY}-shared)

set (GRAPHENE_EXAMPLE OFF CACHE BOOL "Build example")
set (GRAPHENE_DOCUMENTATION OFF CACHE BOOL "Build documentation")
set (GRAPHENE_TEST OFF CACHE BOOL "Build unit tests")

if (GRAPHENE_EXAMPLE)
    add_subdirectory (example)
    install (DIRECTORY example DESTINATION share/${GRAPHENE_LIBRARY})
endif ()

if (GRAPHENE_DOCUMENTATION)
    find_package (Doxygen)
    if (DOXYGEN_FOUND)
        add_custom_target (doxygen ALL doxygen)
        configure_file (doc/Doxyfile.in Doxyfile @ONLY)
        install (DIRECTORY ${PROJECT_BINARY_DIR}/html
                 DESTINATION share/doc/${GRAPHENE_LIBRARY}-${GRAPHENE_VERSION})
    else ()
        message (FATAL_ERROR "Failed to locate doxygen!")
    endif ()
endif ()

if (GRAPHENE_TEST)
    enable_testing ()
    add_subdirectory (test)
endif ()

configure_file (graphene.pc.in graphene.pc @ONLY)
install (FILES ${PROJECT_BINARY_DIR}/graphene.pc DESTINATION lib/pkgconfig)

add_library (${GRAPHENE_LIBRARY} OBJECT ${GRAPHENE_SOURCES})
add_library (${GRAPHENE_STATIC} STATIC $<TARGET_OBJECTS:${GRAPHENE_LIBRARY}>)
add_library (${GRAPHENE_SHARED} SHARED $<TARGET_OBJECTS:${GRAPHENE_LIBRARY}>)

target_link_libraries (${GRAPHENE_STATIC} ${MATH_LIBRARIES})
target_link_libraries (${GRAPHENE_STATIC} ${OPENGL_LIBRARIES})
target_link_libraries (${GRAPHENE_STATIC} ${GLEW_LIBRARIES})
target_link_libraries (${GRAPHENE_STATIC} ${SDL2_LIBRARIES})
target_link_libraries (${GRAPHENE_STATIC} ${SDL2_IMAGE_LIBRARIES})

target_link_libraries (${GRAPHENE_SHARED} ${MATH_LIBRARIES})
target_link_libraries (${GRAPHENE_SHARED} ${OPENGL_LIBRARIES})
target_link_libraries (${GRAPHENE_SHARED} ${GLEW_LIBRARIES})
target_link_libraries (${GRAPHENE_SHARED} ${SDL2_LIBRARIES})
target_link_libraries (${GRAPHENE_SHARED} ${SDL2_IMAGE_LIBRARIES})

# Required to reuse OBJECT for SHARED target, cmake version 2.8.9
set_property (TARGET ${GRAPHENE_LIBRARY} PROPERTY POSITION_INDEPENDENT_CODE 1)
set_target_properties (${GRAPHENE_STATIC} ${GRAPHENE_SHARED} PROPERTIES
                       OUTPUT_NAME ${GRAPHENE_LIBRARY})
set_target_properties (${GRAPHENE_SHARED} PROPERTIES
                       VERSION ${GRAPHENE_VERSION}
                       SOVERSION ${GRAPHENE_VERSION})

install (TARGETS ${GRAPHENE_STATIC} ${GRAPHENE_SHARED} DESTINATION lib)
install (FILES ${GRAPHENE_HEADERS} DESTINATION include/${GRAPHENE_LIBRARY})
