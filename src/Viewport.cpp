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

#include <Viewport.h>
#include <Logger.h>
#include <RenderManager.h>
#include <SceneNode.h>

namespace Graphene {

Viewport::Viewport(int left, int top, int width, int height):
        left(left),
        top(top),
        width(width),
        height(height) {
}

int Viewport::getLeft() const {
    return this->left;
}

int Viewport::getTop() const {
    return this->top;
}

int Viewport::getWidth() const {
    return this->width;
}

int Viewport::getHeight() const {
    return this->height;
}

std::shared_ptr<Camera> Viewport::getCamera() {
    return this->camera;
}

void Viewport::setCamera(const std::shared_ptr<Camera> camera) {
    if (camera == nullptr) {
        throw std::invalid_argument(LogFormat("Camera cannot be nullptr"));
    }

    this->camera = camera;
    this->camera->setAspectRatio(static_cast<float>(this->width) / static_cast<float>(this->height));
}

void Viewport::update() {
    if (this->camera == nullptr) {
        return;
    }

    auto parent = this->camera->getParent();
    if (parent == nullptr) {
        return;
    }

    auto setupViewport = [this]() {
        glViewport(this->left, this->top, this->width, this->height);
    };

    auto& renderManager = GetRenderManager();
    renderManager.pushState(std::make_pair("Setup Viewport", setupViewport));

    int frameWidth = this->width - this->left;
    int frameHeight = this->height - this->top;
    renderManager.renderIndirect(this->camera, frameWidth, frameHeight);
}

}  // namespace Graphene
