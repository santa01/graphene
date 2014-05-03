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

namespace Graphene {

void Viewport::update() {
    if (this->camera == nullptr) {
        return;
    }

    auto parent = this->camera->getParent();
    if (parent == nullptr) {
        return;
    }

    if (this->geometryBuffer == nullptr) {  // Defer construction till OpenGL context is ready
        this->geometryBuffer = std::make_shared<GeometryBuffer>(this->width, this->height);
    }

    RenderStack::push([this]() {
        this->geometryBuffer->getDiffuseTexture()->bind(0);
        this->geometryBuffer->getSpecularTexture()->bind(1);
        this->geometryBuffer->getPositionTexture()->bind(2);
        this->geometryBuffer->getNormalTexture()->bind(3);
        this->geometryBuffer->getDepthTexture()->bind(4);
    });

    RenderStack::push([this]() {
        this->geometryBuffer->bind();
        glDisable(GL_BLEND);
    });

    glViewport(this->left, this->top, this->width, this->height);
    parent->getSceneManager()->render(this->camera);
}

}  // namespace Graphene
