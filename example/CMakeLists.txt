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
project (EXAMPLE)

set (EXAMPLE_EXECUTABLE example)

get_filename_component (GRAPHENE_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component (GRAPHENE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)

file (GLOB_RECURSE EXAMPLE_SOURCES src/*.cpp)
include_directories (src ${GRAPHENE_PROJECT_DIR}/src ${MATH_INCLUDE_DIRS} ${SIGNALS_INCLUDE_DIRS})
link_directories (${GRAPHENE_BINARY_DIR})

add_executable (${EXAMPLE_EXECUTABLE} ${EXAMPLE_SOURCES})
set_target_properties (${EXAMPLE_EXECUTABLE} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_compile_options (${EXAMPLE_EXECUTABLE} PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -pedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/WX>
)
target_compile_definitions (${EXAMPLE_EXECUTABLE} PUBLIC _USE_MATH_DEFINES)

target_link_libraries (${EXAMPLE_EXECUTABLE} ${GRAPHENE_STATIC} ${MATH_LIBRARIES})
