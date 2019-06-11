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

#include <RenderTarget.h>
#include <RenderStack.h>
#include <stdexcept>

namespace Graphene {

RenderTarget::RenderTarget(int width, int height) {
    this->buffer = GL_FRONT;
    this->fbo = 0;

    this->width = width;
    this->height = height;
    this->autoUpdate = true;

    auto viewport = std::make_shared<Viewport>(0, 0, this->width, this->height);
    this->viewports.insert(viewport);
}

int RenderTarget::getWidth() const {
    return this->width;
}

int RenderTarget::getHeight() const {
    return this->height;
}

bool RenderTarget::isAutoUpdate() const {
    return this->autoUpdate;
}

void RenderTarget::setAutoUpdate(bool autoUpdate) {
    this->autoUpdate = autoUpdate;
}

const std::unordered_set<std::shared_ptr<Viewport>>& RenderTarget::getViewports() {
    return this->viewports;
}

void RenderTarget::addViewport(const std::shared_ptr<Viewport> viewport) {
    if (viewport == nullptr) {
        throw std::invalid_argument("Viewport cannot be nullptr");
    }

    this->viewports.insert(viewport);
}

const std::unordered_set<std::shared_ptr<Viewport>>& RenderTarget::getOverlays() {
    return this->overlays;
}

void RenderTarget::addOverlay(const std::shared_ptr<Viewport> overlay) {
    if (overlay == nullptr) {
        throw std::invalid_argument("Viewport cannot be nullptr");
    }

    this->overlays.insert(overlay);
}

void RenderTarget::update() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
    glDrawBuffer(this->buffer);
    glClear(GL_COLOR_BUFFER_BIT);

    auto renderState = [this]() {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
        glDrawBuffer(this->buffer);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
    };

    for (auto& viewport: this->viewports) {
        RenderStack::push(renderState);
        viewport->update();
    }

    for (auto& overlay: this->overlays) {
        RenderStack::push(renderState);
        overlay->update();
    }
}

}  // namespace Graphene
