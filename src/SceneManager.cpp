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

#include <SceneManager.h>
#include <RenderStack.h>
#include <Object.h>
#include <Light.h>
#include <Entity.h>
#include <Sources.h>

namespace Graphene {

SceneManager::SceneManager():
        geometryShader(new Shader(Sources::geometryShader, sizeof(Sources::geometryShader))),
        ambientShader(new Shader(Sources::ambientShader, sizeof(Sources::ambientShader))),
        lightingShader(new Shader(Sources::lightingShader, sizeof(Sources::lightingShader))),
        ambientColor(1.0f, 1.0f, 1.0f) {
    this->ambientEnergy = 1.0f;

    this->lightPass = false;
    this->shadowPass = false;

    FrameGeometry geometry = {
        { -1.0f, -1.0f,  0.0f, -1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f,  0.0f },
        {  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f },
        {  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f }, { 0, 1, 3, 1, 2, 3 }
    };
    this->frame = std::make_shared<Mesh>(reinterpret_cast<char*>(&geometry), 2, 4);
}

void SceneManager::render(const std::shared_ptr<Camera> camera) {
    if (camera == nullptr) {
        throw std::invalid_argument("Camera cannot be nullptr");
    }

    if (RenderStack::depth() < 3) {
        throw std::runtime_error("RenderStack is not populated");
    }

    RenderStack::pop();  // Geometry buffer
    this->geometryShader->enable();
    this->geometryShader->setUniformBlock("Material", BIND_MATERIAL);

    this->geometryShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->geometryShader->setUniform("modelViewProjection",
            camera->getProjection() * camera->getRotation() * camera->getTranslation());

    traverseScene(this->rootNode, [this](const std::shared_ptr<Object> object) {
        if (object->getObjectType() == Object::ObjectType::TYPE_ENTITY) {
            auto entity = std::dynamic_pointer_cast<Entity>(object);
            this->geometryShader->setUniform("localWorld",
                    entity->getScaling() * entity->getRotation() * entity->getTranslation());

            for (auto& mesh: entity->getMeshes()) {
                auto material = mesh->getMaterial();
                material->getMaterialBuffer()->bind(BIND_MATERIAL);

                auto diffuseTexture = material->getDiffuseTexture();
                if (diffuseTexture != nullptr) {
                    diffuseTexture->bind(TEXTURE_DIFFUSE);
                }

                mesh->render();
            }
        }
    });

    RenderStack::pop();  // Geometry textures
    this->ambientShader->enable();
    this->ambientShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->ambientShader->setUniform("ambientColor", this->ambientColor);
    this->ambientShader->setUniform("ambientEnergy", this->ambientEnergy);

    RenderStack::pop();  // Output buffer
    this->frame->render();

    if (this->shadowPass) {
        renderShadows();
    }

    if (this->lightPass) {
        renderLights();
    }
}

void SceneManager::renderShadows() {
    // TODO
}

void SceneManager::renderLights() {
    this->lightingShader->enable();
    this->lightingShader->setUniformBlock("Light", BIND_LIGHT);

    this->lightingShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);
    this->lightingShader->setUniform("specularSampler", TEXTURE_SPECULAR);
    this->lightingShader->setUniform("positionSampler", TEXTURE_POSITION);
    this->lightingShader->setUniform("normalSampler", TEXTURE_NORMAL);
    this->lightingShader->setUniform("depthSampler", TEXTURE_DEPTH);

    traverseScene(this->rootNode, [this](const std::shared_ptr<Object> object) {
        if (object->getObjectType() == Object::ObjectType::TYPE_LIGHT) {
            auto light = std::dynamic_pointer_cast<Light>(object);
            light->getLightBuffer()->bind(BIND_LIGHT);

            this->frame->render();
        }
    });
}

}  // namespace Graphene
