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

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <OpenGL.h>

namespace Graphene {

class Mesh: public NonCopyable {
public:
    GRAPHENE_API Mesh(const void* data, int vertices, int faces);
    GRAPHENE_API ~Mesh();

    GRAPHENE_API int getVertices() const;
    GRAPHENE_API int getFaces() const;

    GRAPHENE_API void render();

private:
    GLuint vao = 0;
    GLuint buffers[2] = { };

    int vertices = 0;
    int faces = 0;
};

}  // namespace Graphene

#endif  // MESH_H
