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

#include <Window.h>
#include <OpenGL.h>
#include <RenderManager.h>
#include <algorithm>

namespace Graphene {

Window::Window(int width, int height):
        RenderTarget(width, height) {
}

const KeyboardState& Window::getKeyboardState() const {
    return this->keyboardState;
}

const MouseState& Window::getMouseState() const {
    return this->mouseState;
}

const MousePosition& Window::getMousePosition() const {
    return this->mousePosition;
}

bool Window::isMouseCaptured() const {
    return this->mouseCaptured;
}

const std::unordered_set<std::shared_ptr<Viewport>>& Window::getOverlays() const {
    return this->overlays;
}

std::shared_ptr<Viewport> Window::createOverlay(int left, int top, int width, int height) {
    auto overlay = std::make_shared<Viewport>(left, top, width, height);
    this->overlays.insert(overlay);

    return overlay;
}

std::shared_ptr<Viewport> Window::createViewport(int left, int top, int width, int height) {
    auto geometryBuffer = std::make_shared<GeometryBuffer>(width, height);
    auto geometryViewport = geometryBuffer->createViewport(0, 0, width, height);

    this->geometryBuffers.insert(geometryBuffer);
    return RenderTarget::createViewport(left, top, width, height);
}

void Window::update() {
    for (auto& viewport: this->viewports) {
        auto geometryFinder = [&viewport](std::shared_ptr<GeometryBuffer> buffer) {
            return buffer->getWidth() == viewport->getWidth() && buffer->getHeight() == viewport->getHeight();
        };

        auto geometryBuffer = *std::find_if(this->geometryBuffers.begin(), this->geometryBuffers.end(), geometryFinder);
        auto geometryViewport = *geometryBuffer->getViewports().begin();
        geometryViewport->setCamera(viewport->getCamera());

        geometryBuffer->update();
        geometryBuffer->getDiffuseTexture()->bind(TEXTURE_DIFFUSE);
        geometryBuffer->getSpecularTexture()->bind(TEXTURE_SPECULAR);
        geometryBuffer->getPositionTexture()->bind(TEXTURE_POSITION);
        geometryBuffer->getNormalTexture()->bind(TEXTURE_NORMAL);
        geometryBuffer->getDepthTexture()->bind(TEXTURE_DEPTH);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo);
        glDrawBuffer(GL_BACK);  // Double buffered

        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        GetRenderManager().setRenderStep(RenderStep::FRAME);
        viewport->update();
    }

    for (auto& overlay: this->overlays) {
        GetRenderManager().setRenderStep(RenderStep::BUFFER);
        overlay->update();
    }

    this->swapBuffers();
}

}  // namespace Graphene
