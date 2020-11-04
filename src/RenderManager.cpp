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
    auto shader = objectManager.createShader();

    this->renderStates = {
        { RenderStateType::GEOMETRY, std::make_shared<RenderGeometry>() },
        { RenderStateType::OVERLAY,  std::make_shared<RenderOverlay>() },
        { RenderStateType::BUFFER,   std::make_shared<RenderBuffer>() },
        { RenderStateType::SKYBOX,   std::make_shared<RenderSkybox>() },
        { RenderStateType::FRAME,    std::make_shared<RenderFrame>() },
        { RenderStateType::SHADOWS,  std::make_shared<RenderShadows>() },
        { RenderStateType::LIGHTS,   std::make_shared<RenderLights>() },
        { RenderStateType::NONE,     std::make_shared<RenderNone>() }
    };

    this->getRenderState(RenderStateType::NONE)->setShader(shader);
    this->setRenderState(RenderStateType::NONE);
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

std::shared_ptr<Mesh> RenderManager::getFrame() const {
    return this->frame;
}

void RenderManager::setRenderState(RenderStateType stateType) {
    this->renderState = this->renderStates.at(stateType);
    this->renderState->enter(this);
}

std::shared_ptr<RenderState> RenderManager::getRenderState(RenderStateType stateType) const {
    return this->renderStates.at(stateType);
}

void RenderManager::update(const std::shared_ptr<Camera> camera) {
    if (camera == nullptr) {
        throw std::invalid_argument(LogFormat("Camera cannot be nullptr"));
    }

    std::shared_ptr<RenderState> renderNone = this->renderStates.at(RenderStateType::NONE);
    while (this->renderState != renderNone) {
        this->setRenderState(this->renderState->update(this, camera));
    }
}

void RenderManager::teardown() {
    this->renderStates.clear();
}

}  // namespace Graphene
