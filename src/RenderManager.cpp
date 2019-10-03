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
#include <ObjectManager.h>
#include <Logger.h>
#include <Scene.h>
#include <Light.h>
#include <Entity.h>
#include <Mat4.h>
#include <stdexcept>

namespace Graphene {

enum BindPoints {
    BIND_MATERIAL = 0,
    BIND_LIGHT = 0
};

RenderManager& RenderManager::getInstance() {
    static RenderManager instance;
    return instance;
}

RenderManager::RenderManager() {
    this->shaders = {
        { RenderStep::GEOMETRY, nullptr },
        { RenderStep::FRAME,    nullptr },
        { RenderStep::SHADOWS,  nullptr },
        { RenderStep::LIGHTS,   nullptr },
        { RenderStep::OVERLAY,  nullptr },
        { RenderStep::BUFFER,   nullptr },
    };
    this->renderers = {
        { RenderStep::GEOMETRY, std::bind(&RenderManager::renderEntities, this, std::placeholders::_1) },
        { RenderStep::FRAME,    std::bind(&RenderManager::renderFrame,    this, std::placeholders::_1) },
        { RenderStep::SHADOWS,  std::bind(&RenderManager::renderShadows,  this, std::placeholders::_1) },
        { RenderStep::LIGHTS,   std::bind(&RenderManager::renderLights,   this, std::placeholders::_1) },
        { RenderStep::OVERLAY,  std::bind(&RenderManager::renderEntities, this, std::placeholders::_1) },
        { RenderStep::BUFFER,   std::bind(&RenderManager::renderEntities, this, std::placeholders::_1) }
    };
    this->frame = GetObjectManager().createQuad(MeshWinding::WINDING_CLOCKWISE);
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

void RenderManager::setShader(RenderStep step, std::shared_ptr<Shader> shader) {
    this->shaders.at(step) = shader;
}

std::shared_ptr<Shader> RenderManager::getShader(RenderStep step) const {
    return this->shaders.at(step);
}

void RenderManager::setRenderStep(RenderStep step) {
    this->step = step;
}

RenderStep RenderManager::getRenderStep() const {
    return this->step;
}

void RenderManager::render(const std::shared_ptr<Camera> camera) {
    if (camera == nullptr) {
        throw std::invalid_argument(LogFormat("Camera cannot be nullptr"));
    }

    while (this->step != RenderStep::NONE) {
        this->shader = this->shaders.at(this->step);
        this->shader->enable();

        auto renderer = this->renderers.at(this->step);
        this->step = renderer(camera);
    }
}

RenderStep RenderManager::renderEntities(const std::shared_ptr<Camera> camera) {
    this->shader->setUniformBlock("Material", BIND_MATERIAL);
    this->shader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);

    // Modelview -> project
    std::shared_ptr<Scene> scene = camera->getParent()->getScene();
    Math::Mat4 modelViewProjection = camera->getProjection() * Scene::calculateModelView(camera);
    this->shader->setUniform("modelViewProjection", modelViewProjection);

    scene->iterateEntities([this](const std::shared_ptr<Entity> entity, const Math::Mat4& localWorld, const Math::Mat4& normalRotation) {
        this->shader->setUniform("objectId", entity->getId());
        this->shader->setUniform("localWorld", localWorld);
        this->shader->setUniform("normalRotation", normalRotation);

        for (auto& mesh: entity->getMeshes()) {
            auto material = mesh->getMaterial();
            material->getMaterialBuffer()->bind(BIND_MATERIAL);

            auto diffuseTexture = material->getDiffuseTexture();
            if (diffuseTexture != nullptr) {
                diffuseTexture->bind(TEXTURE_DIFFUSE);
            }

            mesh->render();
        }
    });

    return RenderStep::NONE;
}

RenderStep RenderManager::renderFrame(const std::shared_ptr<Camera> camera) {
    std::shared_ptr<Scene> scene = camera->getParent()->getScene();

    this->shader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->shader->setUniform("ambientColor", scene->getAmbientColor());
    this->shader->setUniform("ambientEnergy", scene->getAmbientEnergy());

    this->frame->render();

    if (this->shadowPass) {
        return RenderStep::SHADOWS;
    }
    if (this->lightPass) {
        return RenderStep::LIGHTS;
    }

    return RenderStep::NONE;
}

RenderStep RenderManager::renderShadows(const std::shared_ptr<Camera> /*camera*/) {
    if (this->lightPass) {
        return RenderStep::LIGHTS;
    }

    return RenderStep::NONE;
}

RenderStep RenderManager::renderLights(const std::shared_ptr<Camera> camera) {
    this->shader->setUniformBlock("Light", BIND_LIGHT);

    this->shader->setUniform("specularSampler", TEXTURE_SPECULAR);
    this->shader->setUniform("positionSampler", TEXTURE_POSITION);
    this->shader->setUniform("normalSampler", TEXTURE_NORMAL);
    this->shader->setUniform("depthSampler", TEXTURE_DEPTH);

    this->shader->setUniform("cameraPosition", Scene::calculatePosition(camera));

    std::shared_ptr<Scene> scene = camera->getParent()->getScene();
    scene->iterateLights([this](const std::shared_ptr<Light> light, const Math::Vec3& position, const Math::Vec3& direction) {
        light->getLightBuffer()->bind(BIND_LIGHT);

        this->shader->setUniform("lightPosition", position);
        this->shader->setUniform("lightDirection", direction);

        this->frame->render();
    });

    return RenderStep::NONE;
}

}  // namespace Graphene
