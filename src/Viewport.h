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

#include <NonCopyable.h>
#include <Camera.h>
#include <SceneNode.h>
#include <SceneManager.h>
#include <GL/glew.h>
#include <stdexcept>
#include <memory>

namespace Graphene {

class Viewport: public NonCopyable {
public:
    Viewport(int left, int top, int width, int height) {
        this->left = left;
        this->top = top;
        this->width = width;
        this->height = height;
    }

    std::shared_ptr<Camera> getCamera() {
        return this->camera;
    }

    void setCamera(const std::shared_ptr<Camera> camera) {
        if (camera == nullptr) {
            throw std::invalid_argument("Camera cannot be nullptr");
        }

        this->camera = camera;
    }

    int getLeft() const {
        return this->left;
    }

    void setLeft(int left) {
        this->left = left;
    }

    int getTop() const {
        return this->top;
    }

    void setTop(int top) {
        this->top = top;
    }

    int getWidth() const {
        return this->width;
    }

    void setWidth(int width) {
        this->width = width;
    }

    int getHeight() const {
        return this->height;
    }

    void setHeight(int height) {
        this->height = height;
    }

    void update() {
        if (this->camera == nullptr) {
            return;
        }

        auto parent = this->camera->getParent();
        if (parent == nullptr) {
            return;
        }

        glViewport(this->left, this->top, this->width, this->height);
        parent->getSceneManager()->render(this->camera);
    }

private:
    std::shared_ptr<Camera> camera;

    int left;
    int top;
    int width;
    int height;
};

}  // namespace Graphene

#endif  // VIEWPORT_H
