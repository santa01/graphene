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

#include <RenderTarget.h>
#include <ImageTexture.h>
#include <DepthTexture.h>
#include <GL/glew.h>
#include <unordered_set>
#include <stdexcept>
#include <memory>

namespace Graphene {

class FrameBuffer: public RenderTarget {
public:
    FrameBuffer(int width, int height):
            RenderTarget(width, height),
            depthAttachement(new DepthTexture(width, height)) {
        glGenFramebuffers(1, &this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthAttachement->texture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~FrameBuffer() {
        glDeleteFramebuffers(1, &this->fbo);
    }

    const std::unordered_set<std::shared_ptr<ImageTexture>>& getColorAttachements() {
        return this->colorAttachements;
    }

    void addColorAttachement(const std::shared_ptr<ImageTexture> colorAttachement) {
        if (colorAttachement == nullptr) {
           throw std::invalid_argument("ImageTexture cannot be nullptr");
        }

        if (this->colorAttachements.find(colorAttachement) == this->colorAttachements.end()) {
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + this->colorAttachements.size(),
                    colorAttachement->texture, 0);
            this->colorAttachements.insert(colorAttachement);
        }
    }

    std::shared_ptr<DepthTexture> getDepthAttachement() {
        return this->depthAttachement;
    }

private:
    std::unordered_set<std::shared_ptr<ImageTexture>> colorAttachements;
    std::shared_ptr<DepthTexture> depthAttachement;
};

}  // namespace Graphene

#endif  // FRAMEBUFFER_H
