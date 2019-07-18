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
#include <Mat4.h>
#include <stdexcept>
#include <algorithm>

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

RenderManager::RenderManager() {
    FrameGeometry geometry = {
        { -1.0f, -1.0f,  0.0f, -1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f,  0.0f },
        {  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f },
        {  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f },
        { 0, 1, 3, 1, 2, 3 }
    };
    this->frame = std::make_shared<Mesh>(&geometry, 2, 4);
}

void RenderManager::enableShader(std::shared_ptr<Shader> shader) {
    this->activeShader = shader;
    this->activeShader->enable();
}

std::shared_ptr<GeometryBuffer> RenderManager::getGeometryBuffer(int width, int height) {
    auto geometryBuffer = std::find_if(this->geometryBuffers.begin(), this->geometryBuffers.end(),
        [width, height](std::shared_ptr<GeometryBuffer> buffer) {
            return buffer->getWidth() == width && buffer->getHeight() == height;
        });

    if (geometryBuffer == this->geometryBuffers.end()) {
        this->geometryBuffers.push_back(std::make_shared<GeometryBuffer>(width, height));
        geometryBuffer = --this->geometryBuffers.end();
    }

    return *geometryBuffer;
}

std::shared_ptr<Shader> RenderManager::getGeometryOutputShader() {
    return this->geometryOutputShader;
}

void RenderManager::setGeometryOutputShader(const std::shared_ptr<Shader> shader) {
    if (shader == nullptr) {
        throw std::invalid_argument(LogFormat("Shader cannot be nullptr"));
    }

    this->geometryOutputShader = shader;
}

std::shared_ptr<Shader> RenderManager::getAmbientLightingShader() {
    return this->ambientLightingShader;
}

void RenderManager::setAmbientLightingShader(const std::shared_ptr<Shader> shader) {
    if (shader == nullptr) {
        throw std::invalid_argument(LogFormat("Shader cannot be nullptr"));
    }

    this->ambientLightingShader = shader;
}

std::shared_ptr<Shader> RenderManager::getDeferredLightingShader() {
    return this->deferredLightingShader;
}

void RenderManager::setDeferredLightingShader(const std::shared_ptr<Shader> shader) {
    if (shader == nullptr) {
        throw std::invalid_argument(LogFormat("Shader cannot be nullptr"));
    }

    this->deferredLightingShader = shader;
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

void RenderManager::renderDirect(const std::shared_ptr<Camera> camera) {
    if (camera == nullptr) {
        throw std::invalid_argument(LogFormat("Camera cannot be nullptr"));
    }

    this->popState();  // Setup viewport
    this->popState();  // Bind target buffer

    this->enableShader(this->ambientLightingShader);
    this->renderEntities(camera);
}

void RenderManager::renderIndirect(const std::shared_ptr<Camera> camera, int frameWidth, int frameHeight) {
    if (camera == nullptr) {
        throw std::invalid_argument(LogFormat("Camera cannot be nullptr"));
    }

    auto geometryBuffer = this->getGeometryBuffer(frameWidth, frameHeight);
    geometryBuffer->bind();

    this->enableShader(this->geometryOutputShader);
    this->renderEntities(camera);

    geometryBuffer->getDiffuseTexture()->bind(TEXTURE_DIFFUSE);
    geometryBuffer->getSpecularTexture()->bind(TEXTURE_SPECULAR);
    geometryBuffer->getPositionTexture()->bind(TEXTURE_POSITION);
    geometryBuffer->getNormalTexture()->bind(TEXTURE_NORMAL);
    geometryBuffer->getDepthTexture()->bind(TEXTURE_DEPTH);

    this->popState();  // Setup viewport
    this->popState();  // Bind target buffer

    this->enableShader(this->ambientLightingShader);
    this->renderFrame(camera);

    if (this->shadowPass) {
        // this->enableShader(this->shadowMapShader);
        this->renderShadows(camera);
    }

    if (this->lightPass) {
        this->enableShader(this->deferredLightingShader);
        this->renderLights(camera);
    }
}

void RenderManager::renderEntities(const std::shared_ptr<Camera> camera) {
    this->activeShader->setUniformBlock("Material", BIND_MATERIAL);
    this->activeShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);

    // Modelview -> project
    std::shared_ptr<Scene> scene = camera->getParent()->getScene();
    Math::Mat4 modelViewProjection = camera->getProjection() * scene->calculateModelView(camera);
    this->activeShader->setUniform("modelViewProjection", modelViewProjection);

    scene->iterateEntities([this](const std::shared_ptr<Entity> entity, const Math::Mat4& localWorld) {
        this->activeShader->setUniform("normalRotation", entity->getRotation());
        this->activeShader->setUniform("localWorld", localWorld);

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
}

void RenderManager::renderFrame(const std::shared_ptr<Camera> camera) {
    std::shared_ptr<Scene> scene = camera->getParent()->getScene();

    this->activeShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->activeShader->setUniform("ambientColor", scene->getAmbientColor());
    this->activeShader->setUniform("ambientEnergy", scene->getAmbientEnergy());

    this->frame->render();
}

void RenderManager::renderShadows(const std::shared_ptr<Camera> /*camera*/) {
    // TODO
}

void RenderManager::renderLights(const std::shared_ptr<Camera> camera) {
    this->activeShader->setUniformBlock("Light", BIND_LIGHT);

    this->activeShader->setUniform("specularSampler", TEXTURE_SPECULAR);
    this->activeShader->setUniform("positionSampler", TEXTURE_POSITION);
    this->activeShader->setUniform("normalSampler", TEXTURE_NORMAL);
    this->activeShader->setUniform("depthSampler", TEXTURE_DEPTH);

    this->activeShader->setUniform("cameraPosition", camera->getPosition());

    std::shared_ptr<Scene> scene = camera->getParent()->getScene();
    scene->iterateLights([this](const std::shared_ptr<Light> light, const Math::Vec3& position, const Math::Vec3& rotation) {
        light->getLightBuffer()->bind(BIND_LIGHT);

        this->activeShader->setUniform("lightPosition", position);
        this->activeShader->setUniform("lightDirection", rotation);

        this->frame->render();
    });
}

}  // namespace Graphene
