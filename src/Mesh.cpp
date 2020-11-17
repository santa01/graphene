/*
 * Copyright (c) 2013 Pavlo Lavrenenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Mesh.h>

namespace Graphene {

enum DataBuffer { BUFFER_VERTICES, BUFFER_FACES };
enum VertexAttribute { ATTRIBUTE_POSITION, ATTRIBUTE_NORMAL, ATTRIBUTE_UV };

Mesh::Mesh(const void* data, int vertices, int faces):
        vertices(vertices),
        faces(faces) {
    const char* meshData = reinterpret_cast<const char*>(data);

    const void* vertexData = meshData;
    size_t vertexDataSize = sizeof(float) * this->vertices * (3 + 3 + 2);

    const void* faceData = meshData + vertexDataSize;
    size_t faceDataSize = sizeof(int) * this->faces * 3;

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(2, this->buffers);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers[BUFFER_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(ATTRIBUTE_POSITION);
    glEnableVertexAttribArray(ATTRIBUTE_NORMAL);
    glEnableVertexAttribArray(ATTRIBUTE_UV);

    ptrdiff_t normalDataOffset = sizeof(float) * this->vertices * 3;
    ptrdiff_t uvDataOffset = normalDataOffset * 2;

    glVertexAttribPointer(ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const void*>(normalDataOffset));
    glVertexAttribPointer(ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const void*>(uvDataOffset));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[BUFFER_FACES]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceDataSize, faceData, GL_STATIC_DRAW);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(2, this->buffers);
}

int Mesh::getFaces() const {
    return this->faces;
}

int Mesh::getVertices() const {
    return this->vertices;
}

void Mesh::render() {
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->faces * 3, GL_UNSIGNED_INT, 0);
}

}  // namespace Graphene
