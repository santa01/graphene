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
#include <Logger.h>
#include <Scene.h>
#include <Object.h>
#include <Light.h>
#include <Entity.h>
#include <Sources.h>
#include <Mat4.h>
#include <stdexcept>

namespace Graphene {

enum BindPoints {
    BIND_MATERIAL = 0,
    BIND_LIGHT = 0
};

#pragma pack(push, 1)

typedef struct {
    float positons[12];
    float normals[12];
    float uvs[8];
    int faces[6];
} FrameGeometry;

#pragma pack(pop)

RenderManager& RenderManager::getInstance() {
    static RenderManager instance;
    return instance;
}

RenderManager::RenderManager():
        geometryShader(new Shader(Sources::geometryShader, sizeof(Sources::geometryShader))),
        ambientShader(new Shader(Sources::ambientShader, sizeof(Sources::ambientShader))),
        lightingShader(new Shader(Sources::lightingShader, sizeof(Sources::lightingShader))) {
    FrameGeometry geometry = {
        { -1.0f, -1.0f,  0.0f, -1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f,  0.0f },
        {  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f },
        {  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f },
        { 0, 1, 3, 1, 2, 3 }
    };
    this->frame = std::make_shared<Mesh>(&geometry, 2, 4);
}

std::shared_ptr<Shader> RenderManager::getGeometryShader() {
    return this->geometryShader;
}

void RenderManager::setGeometryShader(const std::shared_ptr<Shader> geometryShader) {
    if (geometryShader == nullptr) {
        throw std::invalid_argument(LogFormat("Shader cannot be nullptr"));
    }

    this->geometryShader = geometryShader;
}

std::shared_ptr<Shader> RenderManager::getAmbientShader() {
    return this->ambientShader;
}

void RenderManager::setAmbientShader(const std::shared_ptr<Shader> ambientShader) {
    if (ambientShader == nullptr) {
        throw std::invalid_argument(LogFormat("Shader cannot be nullptr"));
    }

    this->ambientShader = ambientShader;
}

std::shared_ptr<Shader> RenderManager::getLightingShader() {
    return this->lightingShader;
}

void RenderManager::setLightingShader(const std::shared_ptr<Shader> lightingShader) {
    if (lightingShader == nullptr) {
        throw std::invalid_argument(LogFormat("Shader cannot be nullptr"));
    }

    this->lightingShader = lightingShader;
}

bool RenderManager::hasShadowPass() const {
    return this->shadowPass;
}

void RenderManager::setShadowPass(bool shadowPass) {
    this->shadowPass = shadowPass;
}

bool RenderManager::hasLightPass() const {
    return this->lightPass;
}

void RenderManager::setLightPass(bool lightPass) {
    this->lightPass = lightPass;
}

void RenderManager::pushState(const RenderState& state) {
    this->stateStack.push(state);
}

void RenderManager::popState() {
    RenderState state(this->stateStack.top());
    this->stateStack.pop();
    state.second();
}

void RenderManager::render(const std::shared_ptr<Camera> camera) {
    if (camera == nullptr) {
        throw std::invalid_argument(LogFormat("Camera cannot be nullptr"));
    }

    this->popState();  // Geometry buffer
    this->geometryShader->enable();
    this->geometryShader->setUniformBlock("Material", BIND_MATERIAL);
    this->geometryShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);

    // Modelview -> project
    std::shared_ptr<Scene> scene = camera->getParent()->getScene();
    Math::Mat4 modelViewProjection = camera->getProjection() * scene->calculateModelView(camera);
    this->geometryShader->setUniform("modelViewProjection", modelViewProjection);

    scene->iterateEntities([this](const std::shared_ptr<Entity> entity, const Math::Mat4& localWorld) {
        this->geometryShader->setUniform("normalRotation", entity->getRotation());
        this->geometryShader->setUniform("localWorld", localWorld);

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

    this->popState();  // Geometry output
    this->ambientShader->enable();
    this->ambientShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->ambientShader->setUniform("ambientColor", scene->getAmbientColor());
    this->ambientShader->setUniform("ambientEnergy", scene->getAmbientEnergy());

    this->popState();  // Target frame buffer
    this->frame->render();

    if (this->shadowPass) {
        renderShadows(camera);
    }

    if (this->lightPass) {
        renderLights(camera);
    }
}

void RenderManager::renderShadows(const std::shared_ptr<Camera> /*camera*/) {
    // TODO
}

void RenderManager::renderLights(const std::shared_ptr<Camera> camera) {
    this->lightingShader->enable();
    this->lightingShader->setUniformBlock("Light", BIND_LIGHT);

    this->lightingShader->setUniform("specularSampler", TEXTURE_SPECULAR);
    this->lightingShader->setUniform("positionSampler", TEXTURE_POSITION);
    this->lightingShader->setUniform("normalSampler", TEXTURE_NORMAL);
    this->lightingShader->setUniform("depthSampler", TEXTURE_DEPTH);

    this->lightingShader->setUniform("cameraPosition", camera->getPosition());

    std::shared_ptr<Scene> scene = camera->getParent()->getScene();
    scene->iterateLights([this](const std::shared_ptr<Light> light, const Math::Vec3& position, const Math::Vec3& rotation) {
        light->getLightBuffer()->bind(BIND_LIGHT);

        this->lightingShader->setUniform("lightPosition", position);
        this->lightingShader->setUniform("lightDirection", rotation);

        this->frame->render();
    });
}

}  // namespace Graphene
