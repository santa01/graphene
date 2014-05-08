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

#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <cstddef>
#include <GL/glew.h>

namespace Graphene {

#define updateBuffer(buffer, type, member, data) \
        buffer->update(reinterpret_cast<const char*>(data), sizeof(type::member), offsetof(type, member))

class UniformBuffer {
public:
    UniformBuffer(const char* data, int dataSize) {
        glGenBuffers(1, &this->ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
        glBufferData(GL_UNIFORM_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    ~UniformBuffer() {
        glDeleteBuffers(1, &this->ubo);
    }

    void update(const char* data, int dataSize, int dataOffset) {
        glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, dataOffset, dataSize, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void bind(int bindPoint) {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, this->ubo);
    }

private:
    GLuint ubo;
};

}  // namespace Graphene

#endif  // UNIFORMBUFFER_H
