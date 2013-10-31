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

#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <NonCopyable.h>
#include <GL/glew.h>

namespace Graphene {

class RenderTarget: public NonCopyable {
public:
    RenderTarget(int width, int height) {
        this->width = width;
        this->height = height;
        this->fbo = 0;
    }

    virtual ~RenderTarget() {}

    int getWidth() const {
        return this->width;
    }

    int getHeight() const {
        return this->height;
    }

    void update() {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

protected:
    GLuint fbo;

    int width;
    int height;
};

}  // namespace Graphene

#endif  // RENDERTARGET_H
