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

cmake_minimum_required (VERSION 2.6)
project (EXAMPLE)

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

include (FindPkgConfig)
pkg_search_module (SDL2 REQUIRED sdl2)
pkg_search_module (MATH REQUIRED math)

file (GLOB_RECURSE EXAMPLE_SOURCES src/*.cpp)
include_directories (src ${MATH_INCLUDE_DIRS})

set (EXAMPLE_EXECUTABLE example)
add_executable (${EXAMPLE_EXECUTABLE} ${EXAMPLE_SOURCES})

target_link_libraries (${EXAMPLE_EXECUTABLE} ${MATH_LIBRARIES})
target_link_libraries (${EXAMPLE_EXECUTABLE} ${SDL2_LIBRARIES})

if (DEFINED GRAPHENE_LIBRARY)  # In-source build
    get_filename_component (GRAPHENE_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    get_filename_component (GRAPHENE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)

    link_directories (${GRAPHENE_BINARY_DIR})
    include_directories (${GRAPHENE_PROJECT_DIR}/src)
    target_link_libraries (${EXAMPLE_EXECUTABLE} ${GRAPHENE_SHARED})
else ()  # Normal build
    pkg_search_module (GRAPHENE REQUIRED graphene)
    include_directories (${GRAPHENE_INCLUDE_DIRS})
    target_link_libraries (${EXAMPLE_EXECUTABLE} ${GRAPHENE_LIBRARIES})
endif ()
