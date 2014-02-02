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
#include <Viewport.h>
#include <GL/glew.h>
#include <unordered_set>
#include <stdexcept>
#include <memory>

namespace Graphene {

class RenderTarget: public NonCopyable {
public:
    RenderTarget(int width, int height) {
        this->fbo = 0;
        this->width = width;
        this->height = height;
        this->autoUpdate = true;

        auto viewport = std::make_shared<Viewport>(0, 0, this->width, this->height);
        this->addViewport(viewport);
    }

    virtual ~RenderTarget() {}

    int getWidth() const {
        return this->width;
    }

    int getHeight() const {
        return this->height;
    }

    bool isAutoUpdate() const {
        return this->autoUpdate;
    }

    void setAutoUpdate(bool autoUpdate) {
        this->autoUpdate = autoUpdate;
    }

    const std::unordered_set<std::shared_ptr<Viewport>>& getViewports() {
        return this->viewports;
    }

    void addViewport(const std::shared_ptr<Viewport> viewport) {
        if (viewport == nullptr) {
            throw std::invalid_argument("Viewport cannot be nullptr");
        }

        this->viewports.insert(viewport);
    }

    void update() {
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

        for (auto& viewport: this->viewports) {
            viewport->update();
        }
    }

protected:
    std::unordered_set<std::shared_ptr<Viewport>> viewports;
    GLuint fbo;

    int width;
    int height;
    bool autoUpdate;
};

}  // namespace Graphene

#endif  // RENDERTARGET_H
