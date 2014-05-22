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

project (EXAMPLE)

get_filename_component (GRAPHENE_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component (GRAPHENE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)

file (GLOB_RECURSE EXAMPLE_SOURCES "src/*.cpp")
include_directories ("src" ${GRAPHENE_PROJECT_DIR}/src ${MATH_INCLUDE_DIRS})

set (EXAMPLE_EXECUTABLE "example")
add_executable (${EXAMPLE_EXECUTABLE} ${EXAMPLE_SOURCES})

link_directories (${GRAPHENE_BINARY_DIR})
target_link_libraries (${EXAMPLE_EXECUTABLE} ${GRAPHENE_SHARED})