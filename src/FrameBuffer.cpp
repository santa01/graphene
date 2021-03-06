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

#include <FrameBuffer.h>
#include <RenderManager.h>

namespace Graphene {

FrameBuffer::FrameBuffer(int width, int height, GLenum format):
        RenderTarget(width, height),
        outputTexture(new Texture2D(width, height, format, 1)),
        depthTexture(new DepthTexture(width, height)) {
    glGenFramebuffers(1, &this->fbo);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->outputTexture->getHandle(), 0);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthTexture->getHandle(), 0);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &this->fbo);
}

const std::shared_ptr<Texture>& FrameBuffer::getOutputTexture() const {
    return this->outputTexture;
}

const std::shared_ptr<DepthTexture>& FrameBuffer::getDepthTexture() const {
    return this->depthTexture;
}

void FrameBuffer::getPixel(int x, int y, GLenum pixelFormat, GLenum pixelType, void* pixel) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
    glReadPixels(x, y, 1, 1, pixelFormat, pixelType, pixel);
}

void FrameBuffer::update() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& viewport: this->viewports) {
        GetRenderManager().setRenderState(RenderBuffer::ID);
        viewport->update();
    }
}

}  // namespace Graphene
