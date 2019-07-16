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

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <GeometryBuffer.h>
#include <Camera.h>
#include <memory>

namespace Graphene {

class Viewport: public NonCopyable {
public:
    GRAPHENE_API Viewport(int left, int top, int width, int height);

    GRAPHENE_API int getLeft() const;
    GRAPHENE_API int getTop() const;
    GRAPHENE_API int getWidth() const;
    GRAPHENE_API int getHeight() const;

    GRAPHENE_API std::shared_ptr<Camera> getCamera();
    GRAPHENE_API void setCamera(const std::shared_ptr<Camera> camera);

    GRAPHENE_API void update();

private:
    int left = 0;
    int top = 0;
    int width = 0;
    int height = 0;

    std::shared_ptr<Camera> camera;
    std::shared_ptr<GeometryBuffer> geometryBuffer;
};

}  // namespace Graphene

#endif  // VIEWPORT_H
