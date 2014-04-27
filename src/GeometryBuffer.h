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

#ifndef GEOMETRYBUFFER_H
#define GEOMETRYBUFFER_H

#include <NonCopyable.h>
#include <ImageTexture.h>
#include <DepthTexture.h>
#include <GL/glew.h>
#include <memory>

namespace Graphene {

class GeometryBuffer: public NonCopyable {
public:
    GeometryBuffer(int width, int height):
            diffuseTexture(new ImageTexture(width, height)),
            positionTexture(new ImageTexture(width, height)),
            normalTexture(new ImageTexture(width, height)),
            depthTexture(new DepthTexture(width, height)) {
        this->width = width;
        this->height = height;

        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);

        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->diffuseTexture->getHandle(), 0);
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, this->positionTexture->getHandle(), 0);
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, this->normalTexture->getHandle(), 0);
        glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthTexture->getHandle(), 0);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    ~GeometryBuffer() {
        glDeleteFramebuffers(1, &this->fbo);
    }

    int getWidth() const {
        return this->width;
    }

    int getHeight() const {
        return this->height;
    }

    std::shared_ptr<ImageTexture> getDiffuseTexture() {
        return this->diffuseTexture;
    }

    std::shared_ptr<ImageTexture> getPositionTexture() {
        return this->positionTexture;
    }

    std::shared_ptr<ImageTexture> getNormalTexture() {
        return this->normalTexture;
    }

    std::shared_ptr<DepthTexture> getDepthTexture() {
        return this->depthTexture;
    }

    void bind() {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
        glDrawBuffers(3, this->buffers);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

private:
    std::shared_ptr<ImageTexture> diffuseTexture;
    std::shared_ptr<ImageTexture> positionTexture;
    std::shared_ptr<ImageTexture> normalTexture;
    std::shared_ptr<DepthTexture> depthTexture;

    GLenum buffers[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    GLuint fbo;

    int width;
    int height;
};

}  // namespace Graphene

#endif  // GEOMETRYBUFFER_H
