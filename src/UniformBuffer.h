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

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <OpenGL.h>
#include <cstddef>

namespace Graphene {

class UniformBuffer: public NonCopyable {
public:
    GRAPHENE_API UniformBuffer(const void* data, size_t dataSize);
    GRAPHENE_API ~UniformBuffer();

    GRAPHENE_API void update(const void* data, size_t dataSize);
    GRAPHENE_API void update(const void* data, size_t dataSize, size_t dataOffset);
    GRAPHENE_API void bind(int bindPoint);

private:
    GLuint ubo = 0;
};

}  // namespace Graphene

#endif  // UNIFORMBUFFER_H
