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
project (TEST)

include (FindPkgConfig)
pkg_search_module (CPPUNIT REQUIRED cppunit)

get_filename_component (GRAPHENE_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component (GRAPHENE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)

include_directories (src ${GRAPHENE_PROJECT_DIR}/src ${MATH_INCLUDE_DIRS} ${CPPUNIT_INCLUDE_DIRS})
set (TEST_LINK_LIBRARIES ${MATH_LIBRARIES} ${CPPUNIT_LIBRARIES})
set (TEST_BINARY_DIR ${GRAPHENE_BINARY_DIR}/test)

set (TEST_CAMERA_EXECUTABLE test-camera)
add_test (${TEST_CAMERA_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_CAMERA_EXECUTABLE})
add_executable (${TEST_CAMERA_EXECUTABLE} src/TestCamera.cpp ../src/Camera.cpp)
target_link_libraries (${TEST_CAMERA_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_ENTITY_EXECUTABLE test-entity)
add_test (${TEST_ENTITY_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_ENTITY_EXECUTABLE})
add_executable (${TEST_ENTITY_EXECUTABLE} src/TestEntity.cpp ../src/Entity.cpp)
target_link_libraries (${TEST_ENTITY_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_LIGHT_EXECUTABLE test-light)
add_test (${TEST_LIGHT_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_LIGHT_EXECUTABLE})
add_executable (${TEST_LIGHT_EXECUTABLE} src/TestLight.cpp ../src/Light.cpp)
target_link_libraries (${TEST_LIGHT_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_SCENE_NODE_EXECUTABLE test-scenenode)
add_test (${TEST_SCENE_NODE_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_SCENE_NODE_EXECUTABLE})
add_executable (${TEST_SCENE_NODE_EXECUTABLE} src/TestSceneNode.cpp
                ../src/SceneNode.cpp ../src/Entity.cpp ../src/Light.cpp)
target_link_libraries (${TEST_SCENE_NODE_EXECUTABLE} ${TEST_LINK_LIBRARIES})
