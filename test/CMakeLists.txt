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
project (TEST)

find_path (CPPUNIT_INCLUDE_DIRS NAMES cppunit/TestSuite.h HINTS ENV CPPUNIT_DIR PATH_SUFFIXES include)
find_library (CPPUNIT_LIBRARIES NAMES cppunit cppunit_dll HINTS ENV CPPUNIT_DIR PATH_SUFFIXES lib)
if (NOT CPPUNIT_INCLUDE_DIRS OR NOT CPPUNIT_LIBRARIES)
    message (FATAL_ERROR "Could NOT find CppUnit")
endif ()

get_filename_component (GRAPHENE_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component (GRAPHENE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)

include_directories (BEFORE src/mock)  # Allow to override standard headers
include_directories (src ${GRAPHENE_PROJECT_DIR}/src ${MATH_INCLUDE_DIRS} ${CPPUNIT_INCLUDE_DIRS})

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED YES)
set (CMAKE_CXX_EXTENSIONS NO)

if (UNIX)
    add_compile_options (-Wall -Wextra -Werror -pedantic)
elseif (WIN32)
    add_compile_options (/WX)
endif ()

add_compile_definitions (GRAPHENE_TEST _USE_MATH_DEFINES)

set (TEST_LINK_LIBRARIES ${MATH_LIBRARIES} ${CPPUNIT_LIBRARIES})
set (TEST_BINARY_DIR ${GRAPHENE_BINARY_DIR}/test)

set (TEST_GRAPHENE_SOURCES
     Scalable.cpp Movable.cpp Rotatable.cpp
     Object.cpp Entity.cpp Camera.cpp Light.cpp ObjectGroup.cpp
     UniformBuffer.cpp Logger.cpp)
list (TRANSFORM TEST_GRAPHENE_SOURCES PREPEND ../src/)
add_library (TEST_GRAPHENE_LIBRARY OBJECT ${TEST_GRAPHENE_SOURCES})

set (TEST_MOVABLE_EXECUTABLE test-movable)
add_test (${TEST_MOVABLE_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_MOVABLE_EXECUTABLE})
add_executable (${TEST_MOVABLE_EXECUTABLE} src/TestMovable.cpp $<TARGET_OBJECTS:TEST_GRAPHENE_LIBRARY>)
target_link_libraries (${TEST_MOVABLE_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_SCALABLE_EXECUTABLE test-scalable)
add_test (${TEST_SCALABLE_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_SCALABLE_EXECUTABLE})
add_executable (${TEST_SCALABLE_EXECUTABLE} src/TestScalable.cpp $<TARGET_OBJECTS:TEST_GRAPHENE_LIBRARY>)
target_link_libraries (${TEST_SCALABLE_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_ROTATABLE_EXECUTABLE test-rotatable)
add_test (${TEST_ROTATABLE_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_ROTATABLE_EXECUTABLE})
add_executable (${TEST_ROTATABLE_EXECUTABLE} src/TestRotatable.cpp $<TARGET_OBJECTS:TEST_GRAPHENE_LIBRARY>)
target_link_libraries (${TEST_ROTATABLE_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_OBJECT_EXECUTABLE test-object)
add_test (${TEST_OBJECT_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_OBJECT_EXECUTABLE})
add_executable (${TEST_OBJECT_EXECUTABLE} src/TestObject.cpp $<TARGET_OBJECTS:TEST_GRAPHENE_LIBRARY>)
target_link_libraries (${TEST_OBJECT_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_CAMERA_EXECUTABLE test-camera)
add_test (${TEST_CAMERA_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_CAMERA_EXECUTABLE})
add_executable (${TEST_CAMERA_EXECUTABLE} src/TestCamera.cpp $<TARGET_OBJECTS:TEST_GRAPHENE_LIBRARY>)
target_link_libraries (${TEST_CAMERA_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_ENTITY_EXECUTABLE test-entity)
add_test (${TEST_ENTITY_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_ENTITY_EXECUTABLE})
add_executable (${TEST_ENTITY_EXECUTABLE} src/TestEntity.cpp $<TARGET_OBJECTS:TEST_GRAPHENE_LIBRARY>)
target_link_libraries (${TEST_ENTITY_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_LIGHT_EXECUTABLE test-light)
add_test (${TEST_LIGHT_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_LIGHT_EXECUTABLE})
add_executable (${TEST_LIGHT_EXECUTABLE} src/TestLight.cpp $<TARGET_OBJECTS:TEST_GRAPHENE_LIBRARY>)
target_link_libraries (${TEST_LIGHT_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_OBJECT_GROUP_EXECUTABLE test-objectgroup)
add_test (${TEST_OBJECT_GROUP_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_OBJECT_GROUP_EXECUTABLE})
add_executable (${TEST_OBJECT_GROUP_EXECUTABLE} src/TestObjectGroup.cpp $<TARGET_OBJECTS:TEST_GRAPHENE_LIBRARY>)
target_link_libraries (${TEST_OBJECT_GROUP_EXECUTABLE} ${TEST_LINK_LIBRARIES})
