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

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GrapheneApi.h>
#include <RenderTarget.h>
#include <Texture.h>
#include <OpenGL.h>
#include <memory>

namespace Graphene {

class FrameBuffer: public RenderTarget {
public:
    GRAPHENE_API FrameBuffer(int width, int height, GLenum format);
    GRAPHENE_API ~FrameBuffer();

    GRAPHENE_API const std::shared_ptr<Texture>& getOutputTexture() const;
    GRAPHENE_API const std::shared_ptr<DepthTexture>& getDepthTexture() const;

    GRAPHENE_API void getPixel(int x, int y, GLenum pixelFormat, GLenum pixelType, void* pixel) const;
    GRAPHENE_API void update() override;

private:
    std::shared_ptr<Texture> outputTexture;
    std::shared_ptr<DepthTexture> depthTexture;
};

}  // namespace Graphene

#endif  // FRAMEBUFFER_H
