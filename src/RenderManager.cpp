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

#include <RenderManager.h>
#include <RenderState.h>
#include <ObjectManager.h>
#include <Logger.h>
#include <stdexcept>

namespace Graphene {

RenderManager& RenderManager::getInstance() {
    static RenderManager instance;
    return instance;
}

RenderManager::RenderManager() {
    auto& objectManager = GetObjectManager();

    this->frame = objectManager.createQuad(FaceWinding::WINDING_CLOCKWISE);
    auto& shader = objectManager.createShader();

    this->renderStates = {
        { RenderGeometry::INDEX, std::make_shared<RenderGeometry>() },
        { RenderOverlay::INDEX,  std::make_shared<RenderOverlay>() },
        { RenderBuffer::INDEX,   std::make_shared<RenderBuffer>() },
        { RenderSkybox::INDEX,   std::make_shared<RenderSkybox>() },
        { RenderFrame::INDEX,    std::make_shared<RenderFrame>() },
        { RenderShadows::INDEX,  std::make_shared<RenderShadows>() },
        { RenderLights::INDEX,   std::make_shared<RenderLights>() },
        { RenderNone::INDEX,     std::make_shared<RenderNone>() }
    };

    this->getRenderState(RenderNone::ID)->setShader(shader);
    this->setRenderState(RenderNone::ID);
}

void RenderManager::setShadowPass(bool shadowPass) {
    this->shadowPass = shadowPass;
}

bool RenderManager::hasShadowPass() const {
    return this->shadowPass;
}

void RenderManager::setLightPass(bool lightPass) {
    this->lightPass = lightPass;
}

bool RenderManager::hasLightPass() const {
    return this->lightPass;
}

const std::shared_ptr<Mesh>& RenderManager::getFrame() const {
    return this->frame;
}

void RenderManager::setRenderState(MetaType stateType) {
    this->renderState = this->renderStates.at(MetaIndex(stateType));
    this->renderState->enter(this);
}

const std::shared_ptr<RenderState>& RenderManager::getRenderState(MetaType stateType) const {
    return this->renderStates.at(MetaIndex(stateType));
}

void RenderManager::update(const std::shared_ptr<Camera>& camera) {
    if (camera == nullptr) {
        throw std::invalid_argument(LogFormat("Camera cannot be nullptr"));
    }

    auto renderNone = this->getRenderState(RenderNone::ID);
    while (this->renderState != renderNone) {
        this->setRenderState(this->renderState->update(this, camera));
    }
}

void RenderManager::teardown() {
    this->renderStates.clear();
}

}  // namespace Graphene
