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

std::shared_ptr<Camera> Viewport::getCamera() {
    return this->camera;
}

void Viewport::setCamera(const std::shared_ptr<Camera> camera) {
    if (camera == nullptr) {
        throw std::invalid_argument(LogFormat("Camera cannot be nullptr"));
    }

    this->camera = camera;
    this->camera->setAspectRatio(this->width / (this->height / 1.0f));
}

int Viewport::getLeft() const {
    return this->left;
}

void Viewport::setLeft(int left) {
    this->left = left;
}

int Viewport::getTop() const {
    return this->top;
}

void Viewport::setTop(int top) {
    this->top = top;
}

int Viewport::getWidth() const {
    return this->width;
}

void Viewport::setWidth(int width) {
    this->width = width;
}

int Viewport::getHeight() const {
    return this->height;
}

void Viewport::setHeight(int height) {
    this->height = height;
}

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

    auto bindGeometryBufferTextures = [this]() {
        this->geometryBuffer->getDiffuseTexture()->bind(TEXTURE_DIFFUSE);
        this->geometryBuffer->getSpecularTexture()->bind(TEXTURE_SPECULAR);
        this->geometryBuffer->getPositionTexture()->bind(TEXTURE_POSITION);
        this->geometryBuffer->getNormalTexture()->bind(TEXTURE_NORMAL);
        this->geometryBuffer->getDepthTexture()->bind(TEXTURE_DEPTH);
    };

    auto bindGeometryBuffer = [this]() {
        this->geometryBuffer->bind();

        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    };

    auto& renderManager = GetRenderManager();
    renderManager.pushState(std::make_pair("Bind Geometry Buffer Textures", bindGeometryBufferTextures));
    renderManager.pushState(std::make_pair("Bind Geometry Buffer", bindGeometryBuffer));

    glViewport(this->left, this->top, this->width, this->height);
    renderManager.render(this->camera);
}

}  // namespace Graphene
