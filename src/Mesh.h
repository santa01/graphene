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

#ifndef MESH_H
#define MESH_H

#include <Material.h>
#include <OpenGL.h>
#include <stdexcept>
#include <memory>

namespace Graphene {

class Mesh {
public:
    Mesh(const char* data, int faces, int vertices);

    ~Mesh() {
        glDeleteVertexArrays(1, &this->vao);
        glDeleteBuffers(2, this->buffers);
    }

    std::shared_ptr<Material> getMaterial() {
        return this->material;
    }

    void setMaterial(const std::shared_ptr<Material> material) {
        if (material == nullptr) {
            throw std::invalid_argument("Material cannot be nullptr");
        }

        this->material = material;
    }

    int getFaces() const {
        return this->faces;
    }

    int getVertices() const {
        return this->vertices;
    }

    void render() {
        glBindVertexArray(this->vao);
        glDrawElements(GL_TRIANGLES, this->faces * 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    enum DataBuffer {
        BUFFER_VERTICES,
        BUFFER_ELEMENTS
    };

    std::shared_ptr<Material> material;

    GLuint buffers[2];
    GLuint vao;

    int faces;
    int vertices;
};

}  // namespace Graphene

#endif  // MESH_H
