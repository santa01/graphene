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

#include <Layout.h>
#include <Overlay.h>
#include <Mat4.h>
#include <Vec4.h>
#include <Vec3.h>

namespace Graphene {

std::shared_ptr<Overlay> Layout::getOverlay() const {
    return this->overlay.lock();
}

void Layout::addComponent(const std::shared_ptr<Label> component, int x, int y) {
    ComponentParameters parameters = { x, y, false };
    this->components.push_back(std::make_pair(component, parameters));
}

void Layout::arrangeComponents() {
    /*
     * See viewport transformation details: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glViewport.xhtml
     *
     * X(window) = (X(ndc) + 1) * (width / 2) + x
     * Y(window) = (Y(ndc) + 1) * (height / 2) + x
     *
     * Reverse transformation:
     *
     * X(ndc) = 2 * (X(window) - x) / width - 1
     * Y(ndc) = 2 * (Y(window) - y) / height - 1
     *
     */
    auto overlay = this->getOverlay();
    if (overlay == nullptr) {
        return;
    }

    auto camera = overlay->getCamera();
    if (camera == nullptr) {
        return;
    }

    float overlayLeft = static_cast<float>(overlay->getLeft());
    float overlayTop = static_cast<float>(overlay->getTop());
    float overlayWidth = static_cast<float>(overlay->getWidth());
    float overlayHeight = static_cast<float>(overlay->getHeight());
    float overlayAspectRatio = overlayWidth / overlayHeight;

    Math::Mat4 inversePerspective(camera->getProjection());
    inversePerspective.invert();

    for (auto& componentParameters: this->components) {
        auto& parameters = componentParameters.second;
        if (parameters.arranged) {
            continue;
        }

        auto& component = componentParameters.first;
        float componentWidth = static_cast<float>(component->getWidth());
        float componentHeight = static_cast<float>(component->getHeight());

        // Move component by the half of their dimentions to shift origin to the bottom-left
        float x = parameters.x + componentWidth / 2.0f;
        float y = parameters.y + componentHeight / 2.0f;
        float xNdc = 2.0f * (x - overlayLeft) / overlayWidth - 1.0f;
        float yNdc = 2.0f * (y - overlayTop) / overlayHeight - 1.0f;

        Math::Vec4 ndcPosition(xNdc, yNdc, 1.0f, 1.0f);
        Math::Vec4 worldPosition(inversePerspective * ndcPosition);
        worldPosition.set(Math::Vec4::Z, component->getPosition().get(Math::Vec4::Z));

        component->scaleX(2.0f * overlayAspectRatio * componentWidth / overlayWidth);
        component->scaleY(2.0f * componentHeight / overlayHeight);
        component->translate(worldPosition.extractVec3());
        parameters.arranged = true;
    }
}

}  // namespace Graphene
