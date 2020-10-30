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
        { RenderStep::SKYBOX,   nullptr },
        { RenderStep::FRAME,    nullptr },
        { RenderStep::SHADOWS,  nullptr },
        { RenderStep::LIGHTS,   nullptr },
        { RenderStep::OVERLAY,  nullptr },
        { RenderStep::BUFFER,   nullptr }
    };

    this->callbacks = {
        { RenderStep::GEOMETRY, [](const std::shared_ptr<Object> object) { } },
        { RenderStep::SKYBOX,   [](const std::shared_ptr<Object> object) { } },
        { RenderStep::FRAME,    [](const std::shared_ptr<Object> object) { } },
        { RenderStep::SHADOWS,  [](const std::shared_ptr<Object> object) { } },
        { RenderStep::LIGHTS,   [](const std::shared_ptr<Object> object) { } },
        { RenderStep::OVERLAY,  [](const std::shared_ptr<Object> object) { } },
        { RenderStep::BUFFER,   [](const std::shared_ptr<Object> object) { } }
    };

    this->renderers = {
        { RenderStep::GEOMETRY, std::bind(&RenderManager::renderEntities, this, std::placeholders::_1) },
        { RenderStep::SKYBOX,   std::bind(&RenderManager::renderSkybox,   this, std::placeholders::_1) },
        { RenderStep::FRAME,    std::bind(&RenderManager::renderFrame,    this, std::placeholders::_1) },
        { RenderStep::SHADOWS,  std::bind(&RenderManager::renderShadows,  this, std::placeholders::_1) },
        { RenderStep::LIGHTS,   std::bind(&RenderManager::renderLights,   this, std::placeholders::_1) },
        { RenderStep::OVERLAY,  std::bind(&RenderManager::renderEntities, this, std::placeholders::_1) },
        { RenderStep::BUFFER,   std::bind(&RenderManager::renderEntities, this, std::placeholders::_1) }
    };

    this->defaultShader = std::make_shared<Shader>("{SHADER_VERSION}\n{SHADER_TYPE}\nvoid main() { }\n");

    this->renderShader = this->defaultShader;
    this->renderShader->enable();

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

void RenderManager::setRenderShader(RenderStep renderStep, std::shared_ptr<Shader> renderShader) {
    this->shaders.at(renderStep) = renderShader;
}

std::shared_ptr<Shader> RenderManager::getRenderShader(RenderStep renderStep) const {
    return this->shaders.at(renderStep);
}

void RenderManager::setRenderCallback(RenderStep renderStep, RenderCallback callback) {
    this->callbacks.at(renderStep) = callback;
}

const RenderCallback& RenderManager::getRenderCallback(RenderStep renderStep) const {
    return this->callbacks.at(renderStep);
}

void RenderManager::setRenderStep(RenderStep renderStep) {
    this->renderStep = renderStep;
}

RenderStep RenderManager::getRenderStep() const {
    return this->renderStep;
}

void RenderManager::render(const std::shared_ptr<Camera> camera) {
    if (camera == nullptr) {
        throw std::invalid_argument(LogFormat("Camera cannot be nullptr"));
    }

    while (this->renderStep != RenderStep::NONE) {
        this->renderCallback = this->callbacks.at(this->renderStep);

        this->renderShader = this->shaders.at(this->renderStep);
        this->renderShader->enable();

        Renderer& renderer = this->renderers.at(this->renderStep);
        this->renderStep = renderer(camera);
    }

    this->renderShader = this->defaultShader;
    this->renderShader->enable();
}

RenderStep RenderManager::renderEntities(const std::shared_ptr<Camera> camera) {
    this->renderShader->setUniformBlock("Material", BIND_MATERIAL);
    this->renderShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);

    // Modelview -> project
    std::shared_ptr<Scene> scene = camera->getParent()->getScene();
    Math::Mat4 modelViewProjection = camera->getProjection() * Scene::calculateModelView(camera);
    this->renderShader->setUniform("modelViewProjection", modelViewProjection);

    scene->iterateEntities([this](const std::shared_ptr<Entity> entity, const Math::Mat4& localWorld, const Math::Mat4& normalRotation) {
        this->renderCallback(entity);

        this->renderShader->setUniform("localWorld", localWorld);
        this->renderShader->setUniform("normalRotation", normalRotation);

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

    return RenderStep::SKYBOX;
}

RenderStep RenderManager::renderSkybox(const std::shared_ptr<Camera> camera) {
    std::shared_ptr<Scene> scene = camera->getParent()->getScene();
    std::shared_ptr<Skybox> skybox = scene->getSkybox();

    if (skybox == nullptr) {
        return RenderStep::NONE;
    }

    this->renderCallback(skybox);

    this->renderShader->setUniformBlock("Material", BIND_MATERIAL);
    this->renderShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);

    // Modelview -> project
    Math::Mat4 modelViewProjection = camera->getProjection() * Scene::calculateView(camera);
    this->renderShader->setUniform("modelViewProjection", modelViewProjection);

    for (auto& mesh: skybox->getMeshes()) {
        auto material = mesh->getMaterial();
        material->getMaterialBuffer()->bind(BIND_MATERIAL);

        auto diffuseTexture = material->getDiffuseTexture();
        if (diffuseTexture != nullptr) {
            diffuseTexture->bind(TEXTURE_DIFFUSE);
        }

        mesh->render();
    }

    return RenderStep::NONE;
}

RenderStep RenderManager::renderFrame(const std::shared_ptr<Camera> camera) {
    std::shared_ptr<Scene> scene = camera->getParent()->getScene();

    this->renderCallback(nullptr);

    this->renderShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->renderShader->setUniform("ambientColor", scene->getAmbientColor());
    this->renderShader->setUniform("ambientEnergy", scene->getAmbientEnergy());

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
    this->renderCallback(nullptr);

    if (this->lightPass) {
        return RenderStep::LIGHTS;
    }

    return RenderStep::NONE;
}

RenderStep RenderManager::renderLights(const std::shared_ptr<Camera> camera) {
    this->renderShader->setUniformBlock("Light", BIND_LIGHT);

    this->renderShader->setUniform("specularSampler", TEXTURE_SPECULAR);
    this->renderShader->setUniform("positionSampler", TEXTURE_POSITION);
    this->renderShader->setUniform("normalSampler", TEXTURE_NORMAL);

    this->renderShader->setUniform("cameraPosition", Scene::calculatePosition(camera));

    std::shared_ptr<Scene> scene = camera->getParent()->getScene();
    scene->iterateLights([this](const std::shared_ptr<Light> light, const Math::Vec3& position, const Math::Vec3& direction) {
        this->renderCallback(light);

        this->renderShader->setUniform("lightPosition", position);
        this->renderShader->setUniform("lightDirection", direction);

        light->getLightBuffer()->bind(BIND_LIGHT);

        this->frame->render();
    });

    return RenderStep::NONE;
}

}  // namespace Graphene
