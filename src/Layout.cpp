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
#include <QuadComponent.h>
#include <Mat4.h>
#include <Vec4.h>
#include <Vec3.h>

namespace Graphene {

const std::shared_ptr<Overlay> Layout::getOverlay() const {
    return this->overlay.lock();
}

void Layout::addEntity(const std::shared_ptr<Entity>& entity, int x, int y) {
    auto quadComponent = entity->getComponent<QuadComponent>();
    if (quadComponent != nullptr) {
        this->entities.emplace(entity, std::make_pair(x, y));
    }
}

void Layout::removeEntity(const std::shared_ptr<Entity>& entity) {
    auto entityPositionsIt = this->entities.find(entity);
    if (entityPositionsIt != this->entities.end()) {
        this->entities.erase(entityPositionsIt);
    }
}

void Layout::arrange() {
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
    if (this->entities.empty()) {
        return;
    }

    auto overlay = this->getOverlay();
    if (overlay == nullptr) {
        return;
    }

    auto& camera = overlay->getCamera();
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

    for (auto& entityPosition: this->entities) {
        auto& entity = entityPosition.first;
        auto& position = entityPosition.second;
        auto quadComponent = entity->getComponent<QuadComponent>();

        auto& origin = quadComponent->getOrigin();
        float x = static_cast<float>(position.first + origin.first);
        float y = static_cast<float>(position.second + origin.second);
        float xNdc = 2.0f * (x - overlayLeft) / overlayWidth - 1.0f;
        float yNdc = 2.0f * (y - overlayTop) / overlayHeight - 1.0f;

        Math::Vec4 ndcPosition(xNdc, yNdc, 1.0f, 1.0f);
        Math::Vec4 worldPosition(inversePerspective * ndcPosition);
        worldPosition.set(Math::Vec4::Z, entity->getPosition().get(Math::Vec4::Z));
        entity->translate(worldPosition.extractVec3());

        // Reset scaling factors to 1.0f, reset initial scaling
        Math::Vec3 scalingFactors(entity->getScalingFactors());
        float scaleX = 1.0f / scalingFactors.get(Math::Vec3::X);
        float scaleY = 1.0f / scalingFactors.get(Math::Vec3::Y);
        float scaleZ = 1.0f / scalingFactors.get(Math::Vec3::Z);

        float width = static_cast<float>(quadComponent->getWidth());
        scaleX *= 2.0f * overlayAspectRatio * width / overlayWidth;

        float height = static_cast<float>(quadComponent->getHeight());
        scaleY *= 2.0f * height / overlayHeight;

        entity->scale(scaleX, scaleY, scaleZ);
    }

    this->entities.clear();
}

}  // namespace Graphene
