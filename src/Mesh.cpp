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
#include <Logger.h>
#include <stdexcept>

namespace Graphene {

enum DataBuffer {
    BUFFER_VERTICES,
    BUFFER_ELEMENTS
};

enum VertexAttributes {
    VERTEX_POSITION,
    VERTEX_NORMAL,
    UV_COORDINATE
};

Mesh::Mesh(const void* data, int vertices, int faces):
        vertices(vertices),
        faces(faces),
        material(new Material()) {
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

    glEnableVertexAttribArray(VERTEX_POSITION);
    glEnableVertexAttribArray(VERTEX_NORMAL);
    glEnableVertexAttribArray(UV_COORDINATE);

    ptrdiff_t normalDataOffset = sizeof(float) * this->vertices * 3;
    ptrdiff_t uvDataOffset = normalDataOffset * 2;

    glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const void*>(normalDataOffset));
    glVertexAttribPointer(UV_COORDINATE, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const void*>(uvDataOffset));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffers[BUFFER_ELEMENTS]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceDataSize, faceData, GL_STATIC_DRAW);

    glBindVertexArray(0);
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

std::shared_ptr<Material> Mesh::getMaterial() const {
    return this->material;
}

void Mesh::setMaterial(const std::shared_ptr<Material> material) {
    if (material == nullptr) {
        throw std::invalid_argument(LogFormat("Material cannot be nullptr"));
    }

    this->material = material;
}

void Mesh::render() {
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->faces * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

}  // namespace Graphene
