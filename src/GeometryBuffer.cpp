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

#include <GeometryBuffer.h>
#include <RenderManager.h>

namespace Graphene {

GeometryBuffer::GeometryBuffer(int width, int height):
        RenderTarget(width, height),
        diffuseTexture(new GeometryTexture(width, height)),
        specularTexture(new GeometryTexture(width, height)),
        positionTexture(new GeometryTexture(width, height)),
        normalTexture(new GeometryTexture(width, height)),
        depthTexture(new DepthTexture(width, height)) {
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);

    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->diffuseTexture->getHandle(), 0);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, this->specularTexture->getHandle(), 0);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, this->positionTexture->getHandle(), 0);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, this->normalTexture->getHandle(), 0);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthTexture->getHandle(), 0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

GeometryBuffer::~GeometryBuffer() {
    glDeleteFramebuffers(1, &this->fbo);
}

std::shared_ptr<GeometryTexture> GeometryBuffer::getDiffuseTexture() const {
    return this->diffuseTexture;
}

std::shared_ptr<GeometryTexture> GeometryBuffer::getSpecularTexture() const {
    return this->specularTexture;
}

std::shared_ptr<GeometryTexture> GeometryBuffer::getPositionTexture() const {
    return this->positionTexture;
}

std::shared_ptr<GeometryTexture> GeometryBuffer::getNormalTexture() const {
    return this->normalTexture;
}

std::shared_ptr<DepthTexture> GeometryBuffer::getDepthTexture() const {
    return this->depthTexture;
}

void GeometryBuffer::update() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);

    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, drawBuffers);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    GetRenderManager().setRenderStep(RenderStep::GEOMETRY);
    for (auto& viewport: this->viewports) {
        viewport->update();
    }
}

}  // namespace Graphene
