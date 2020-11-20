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

#include <RenderState.h>
#include <RenderManager.h>
#include <GraphicsComponent.h>
#include <Texture.h>
#include <Logger.h>
#include <Scene.h>
#include <Light.h>
#include <Entity.h>
#include <Mat4.h>
#include <stdexcept>

namespace Graphene {

void RenderState::setShader(const std::shared_ptr<Shader>& shader) {
    this->shader = shader;
}

const std::shared_ptr<Shader>& RenderState::getShader() const {
    return this->shader;
}

void RenderState::setCallback(const RenderStateCallback& callback) {
    this->callback = callback;
}

const RenderStateCallback& RenderState::getCallback() const {
    return this->callback;
}

void RenderState::enter(RenderManager* /*renderManager*/) {
    this->shader->enable();
}

MetaType RenderState::update(RenderManager* /*renderManager*/, const std::shared_ptr<Camera>& camera) {
    auto scene = camera->getScene();

    this->shader->setUniformBlock("Material", BIND_MATERIAL);
    this->shader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->shader->setUniform("modelViewProjection", camera->getProjection() * Scene::calculateModelView(camera));

    scene->iterateEntities([this](const std::shared_ptr<Entity>& entity, const Math::Mat4& localWorld, const Math::Mat4& normalRotation) {
        this->callback(this, entity);

        this->shader->setUniform("localWorld", localWorld);
        this->shader->setUniform("normalRotation", normalRotation);

        for (auto& component: entity->getComponents()) {
            if (component->isA<GraphicsComponent>()) {
                component->toA<GraphicsComponent>()->render();
            }
        }
    });

    return RenderSkybox::ID;
}

MetaType RenderSkybox::update(RenderManager* /*renderManager*/, const std::shared_ptr<Camera>& camera) {
    auto scene = camera->getScene();
    auto& skybox = scene->getSkybox();

    if (skybox == nullptr) {
        return RenderNone::ID;
    }

    this->callback(this, skybox);

    this->shader->setUniformBlock("Material", BIND_MATERIAL);
    this->shader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->shader->setUniform("modelViewProjection", camera->getProjection() * Scene::calculateView(camera));

    for (auto& component: skybox->getComponents()) {
        if (component->isA<GraphicsComponent>()) {
            component->toA<GraphicsComponent>()->render();
        }
    }

    return RenderNone::ID;
}

MetaType RenderFrame::update(RenderManager* renderManager, const std::shared_ptr<Camera>& camera) {
    auto scene = camera->getScene();
    auto& frame = renderManager->getFrame();

    this->callback(this, nullptr);

    this->shader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->shader->setUniform("ambientColor", scene->getAmbientColor());
    this->shader->setUniform("ambientEnergy", scene->getAmbientEnergy());

    frame->render();

    if (renderManager->hasShadowPass()) {
        return RenderShadows::ID;
    }

    if (renderManager->hasLightPass()) {
        return RenderLights::ID;
    }

    return RenderNone::ID;
}

MetaType RenderShadows::update(RenderManager* renderManager, const std::shared_ptr<Camera>& /*camera*/) {
    if (renderManager->hasLightPass()) {
        return RenderLights::ID;
    }

    return RenderNone::ID;
}

MetaType RenderLights::update(RenderManager* renderManager, const std::shared_ptr<Camera>& camera) {
    auto scene = camera->getScene();
    auto& frame = renderManager->getFrame();

    this->shader->setUniformBlock("Light", BIND_LIGHT);
    this->shader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->shader->setUniform("specularSampler", TEXTURE_SPECULAR);
    this->shader->setUniform("positionSampler", TEXTURE_POSITION);
    this->shader->setUniform("normalSampler", TEXTURE_NORMAL);
    this->shader->setUniform("cameraPosition", Scene::calculatePosition(camera));

    scene->iterateLights([this, &frame](const std::shared_ptr<Light>& light, const Math::Vec3& position, const Math::Vec3& direction) {
        this->callback(this, light);

        this->shader->setUniform("lightPosition", position);
        this->shader->setUniform("lightDirection", direction);

        light->bind(BIND_LIGHT);

        frame->render();
    });

    return RenderNone::ID;
}

MetaType RenderNone::update(RenderManager* /*renderManager*/, const std::shared_ptr<Camera>& /*camera*/) {
    throw std::runtime_error(LogFormat("RenderNone state cannot be updated"));
}

}  // namespace Graphene
