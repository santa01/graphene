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

project (TEST)

add_definitions ("-D__glew_h__")
add_definitions ("-D'GLuint=unsigned int'")

add_definitions ("-DGL_UNIFORM_BUFFER=0")
add_definitions ("-DGL_DYNAMIC_DRAW=0")
add_definitions ("-DGL_TRIANGLES=0")
add_definitions ("-DGL_TEXTURE_2D=0")
add_definitions ("-DGL_UNSIGNED_INT=0")
add_definitions ("-DGL_UNSIGNED_BYTE=0")
add_definitions ("-DGL_RGBA=0")
add_definitions ("-DGL_RGBA16F=0")
add_definitions ("-DGL_LINEAR=0")
add_definitions ("-DGL_LINEAR_MIPMAP_LINEAR=0")
add_definitions ("-DGL_TEXTURE_MIN_FILTER=0")
add_definitions ("-DGL_TEXTURE_MAG_FILTER=0")

add_definitions ("-D'glGenBuffers(x,y)=(void)0'")
add_definitions ("-D'glDeleteBuffers(x,y)=(void)0'")
add_definitions ("-D'glGenTextures(x,y)=(void)0'")
add_definitions ("-D'glDeleteTextures(x,y)=(void)0'")
add_definitions ("-D'glActiveTexture(x)=(void)0'")
add_definitions ("-D'glDeleteVertexArrays(x,y)=(void)0'")
add_definitions ("-D'glBindVertexArray(x)=(void)0'")
add_definitions ("-D'glBindBuffer(x,y)=(void)0'")
add_definitions ("-D'glBindBufferBase(x,y,z)=(void)0'")
add_definitions ("-D'glBufferData(x,y,z,w)=(void)0'")
add_definitions ("-D'glBufferSubData(x,y,z,w)=(void)0'")
add_definitions ("-D'glDrawElements(x,y,z,w)=(void)0'")
add_definitions ("-D'glBindTexture(x,y)=(void)0'")
add_definitions ("-D'glTexParameteri(x,y,z)=(void)0'")
add_definitions ("-D'glTexImage2D(a,b,c,d,e,x,y,z,w)=(void)0'")

get_filename_component (GRAPHENE_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component (GRAPHENE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)

include_directories ("src" ${GRAPHENE_PROJECT_DIR}/src ${MATH_INCLUDE_DIRS} ${CPPUNIT_INCLUDE_DIRS})
set (TEST_LINK_LIBRARIES ${MATH_LIBRARIES} ${CPPUNIT_LIBRARIES})
set (TEST_BINARY_DIR ${GRAPHENE_BINARY_DIR}/test)

set (TEST_CAMERA_EXECUTABLE "TestCamera")
add_test (${TEST_CAMERA_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_CAMERA_EXECUTABLE})
add_executable (${TEST_CAMERA_EXECUTABLE} "src/TestCamera.cpp" "../src/Camera.cpp")
target_link_libraries (${TEST_CAMERA_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_ENTITY_EXECUTABLE "TestEntity")
add_test (${TEST_ENTITY_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_ENTITY_EXECUTABLE})
add_executable (${TEST_ENTITY_EXECUTABLE} "src/TestEntity.cpp" "../src/Entity.cpp")
target_link_libraries (${TEST_ENTITY_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_LIGHT_EXECUTABLE "TestLight")
add_test (${TEST_LIGHT_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_LIGHT_EXECUTABLE})
add_executable (${TEST_LIGHT_EXECUTABLE} "src/TestLight.cpp" "../src/Light.cpp")
target_link_libraries (${TEST_LIGHT_EXECUTABLE} ${TEST_LINK_LIBRARIES})

set (TEST_SCENE_NODE_EXECUTABLE "TestSceneNode")
add_test (${TEST_SCENE_NODE_EXECUTABLE} ${TEST_BINARY_DIR}/${TEST_SCENE_NODE_EXECUTABLE})
add_executable (${TEST_SCENE_NODE_EXECUTABLE} "src/TestSceneNode.cpp"
                "../src/SceneNode.cpp" "../src/Entity.cpp" "../src/Light.cpp")
target_link_libraries (${TEST_SCENE_NODE_EXECUTABLE} ${TEST_LINK_LIBRARIES})
