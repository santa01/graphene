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
#include <Logger.h>
#include <RenderStack.h>
#include <Object.h>
#include <Light.h>
#include <Entity.h>
#include <Sources.h>
#include <Mat4.h>
#include <stdexcept>

namespace Graphene {

SceneManager::SceneManager():
        geometryShader(new Shader(Sources::geometryShader, sizeof(Sources::geometryShader))),
        ambientShader(new Shader(Sources::ambientShader, sizeof(Sources::ambientShader))),
        lightingShader(new Shader(Sources::lightingShader, sizeof(Sources::lightingShader))),
        ambientColor(1.0f, 1.0f, 1.0f) {
    FrameGeometry geometry = {
        { -1.0f, -1.0f,  0.0f, -1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f,  0.0f },
        {  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f },
        {  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f }, { 0, 1, 3, 1, 2, 3 }
    };
    this->frame = std::make_shared<Mesh>(&geometry, 2, 4);
}

std::shared_ptr<SceneNode> SceneManager::createNode() {
    /* const version of shared_from_this() is selected otherwise */
    return std::make_shared<SceneNode>(this->shared_from_this());
}

std::shared_ptr<SceneNode> SceneManager::getRootNode() {
    /* shared_from_this() cannot be called from constructor */
    if (this->rootNode == nullptr) {
        this->rootNode = this->createNode();
    }

    return this->rootNode;
}

std::shared_ptr<Shader> SceneManager::getGeometryShader() {
    return this->geometryShader;
}

void SceneManager::setGeometryShader(const std::shared_ptr<Shader> geometryShader) {
    if (geometryShader == nullptr) {
        throw std::invalid_argument(LogFormat("Shader cannot be nullptr"));
    }

    this->geometryShader = geometryShader;
}

std::shared_ptr<Shader> SceneManager::getAmbientShader() {
    return this->ambientShader;
}

void SceneManager::setAmbientShader(const std::shared_ptr<Shader> ambientShader) {
    if (ambientShader == nullptr) {
        throw std::invalid_argument(LogFormat("Shader cannot be nullptr"));
    }

    this->ambientShader = ambientShader;
}

std::shared_ptr<Shader> SceneManager::getLightingShader() {
    return this->lightingShader;
}

void SceneManager::setLightingShader(const std::shared_ptr<Shader> lightingShader) {
    if (lightingShader == nullptr) {
        throw std::invalid_argument(LogFormat("Shader cannot be nullptr"));
    }

    this->lightingShader = lightingShader;
}

const Math::Vec3& SceneManager::getAmbientColor() const {
    return this->ambientColor;
}

void SceneManager::setAmbientColor(const Math::Vec3& ambientColor) {
    this->ambientColor = ambientColor;
}

float SceneManager::getAmbientEnergy() const {
    return this->ambientEnergy;
}

void SceneManager::setAmbientEnergy(float ambientEnergy) {
    if (ambientEnergy < 0.0f) {
        throw std::runtime_error(LogFormat("Ambient energy is less than 0.0f"));
    }

    this->ambientEnergy = ambientEnergy;
}

bool SceneManager::hasShadowPass() const {
    return this->shadowPass;
}

void SceneManager::setShadowPass(bool shadowPass) {
    this->shadowPass = shadowPass;
}

bool SceneManager::hasLightPass() const {
    return this->lightPass;
}

void SceneManager::setLightPass(bool lightPass) {
    this->lightPass = lightPass;
}

void SceneManager::render(const std::shared_ptr<Camera> camera) {
    if (camera == nullptr) {
        throw std::invalid_argument(LogFormat("Camera cannot be nullptr"));
    }

    RenderStack::pop();  // Geometry buffer
    this->geometryShader->enable();
    this->geometryShader->setUniformBlock("Material", BIND_MATERIAL);
    this->geometryShader->setUniform("diffuseSampler", TEXTURE_DIFFUSE);

    Transformation cameraTransformation = this->calculateTransformation(camera, MASK_TRANSLATION);

    Math::Mat4 cameraTranslation = cameraTransformation.translation;
    cameraTranslation = cameraTranslation * camera->getTranslation();
    cameraTranslation.invert();  // World translates the opposite direction

    // Translate -> rotate -> project
    Math::Mat4 modelViewProjection = camera->getProjection() * camera->getRotation() * cameraTranslation;
    this->geometryShader->setUniform("modelViewProjection", modelViewProjection);

    this->traverseScene([this](const std::shared_ptr<Object> object, const Transformation& transformation) {
        if (object->getType() == ObjectType::ENTITY) {
            auto entity = std::dynamic_pointer_cast<Entity>(object);
            this->geometryShader->setUniform("normalRotation", entity->getRotation());

            // Scale -> rotate -> translate
            Math::Mat4 entityTransformation = entity->getTranslation() * entity->getRotation() * entity->getScaling();
            Math::Mat4 nodesTransformation = transformation.translation * transformation.rotation * transformation.scaling;

            // Local transformations first, than transform the resulting entity
            this->geometryShader->setUniform("localWorld", nodesTransformation * entityTransformation);

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
        renderShadows(camera);
    }

    if (this->lightPass) {
        renderLights(camera);
    }
}

void SceneManager::renderShadows(const std::shared_ptr<Camera> /*camera*/) {
    // TODO
}

void SceneManager::renderLights(const std::shared_ptr<Camera> camera) {
    this->lightingShader->enable();
    this->lightingShader->setUniformBlock("Light", BIND_LIGHT);

    this->lightingShader->setUniform("specularSampler", TEXTURE_SPECULAR);
    this->lightingShader->setUniform("positionSampler", TEXTURE_POSITION);
    this->lightingShader->setUniform("normalSampler", TEXTURE_NORMAL);
    this->lightingShader->setUniform("depthSampler", TEXTURE_DEPTH);

    this->lightingShader->setUniform("cameraPosition", camera->getPosition());

    this->traverseScene([this](const std::shared_ptr<Object> object, const Transformation& transformation) {
        if (object->getType() == ObjectType::LIGHT) {
            auto light = std::dynamic_pointer_cast<Light>(object);
            light->getLightBuffer()->bind(BIND_LIGHT);

            Math::Mat4 lightTranslation = transformation.translation * light->getTranslation();
            Math::Vec3 lightPosition = Math::Vec3(lightTranslation.get(0, 3), lightTranslation.get(1, 3), lightTranslation.get(2, 3));

            this->lightingShader->setUniform("lightPosition", lightPosition);
            this->lightingShader->setUniform("lightDirection", light->getDirection());

            this->frame->render();
        }
    });
}

SceneManager::Transformation SceneManager::calculateTransformation(const std::shared_ptr<Object> object, int calculationMask) {
    std::shared_ptr<SceneNode> node = object->getParent();
    Transformation transformation;

    while (node != nullptr) {
        // Moving to the root node, current node's transformation matrix is the right operand
        // to be the first operation. Eventually root node's transformation will be the first one.

        if (calculationMask & MASK_TRANSLATION) {
            transformation.translation = transformation.translation * node->getTranslation();
        }

        if (calculationMask & MASK_SCALING) {
            transformation.scaling = transformation.scaling * node->getScaling();
        }

        node = node->getParent();
    }

    return transformation;
}

void SceneManager::traverseScene(ObjectHandler handler) {
    std::function<void(const std::shared_ptr<SceneNode>, Transformation)> traverser;
    traverser = [&handler, &traverser](const std::shared_ptr<SceneNode> node, Transformation transformation) {
        // Moving from the root node, current node's transformation matrix is the left operand
        // to be the last operation. Keep the root node's transformation the first one.

        transformation.translation = node->getTranslation() * transformation.translation;
        transformation.scaling = node->getScaling() * transformation.scaling;

        auto objects = node->getObjects();
        std::for_each(objects.begin(), objects.end(), [&handler, &transformation](const std::shared_ptr<Object> object) {
            handler(object, transformation);
        });

        auto nodes = node->getNodes();
        std::for_each(nodes.begin(), nodes.end(), [&traverser, &transformation](const std::shared_ptr<SceneNode> node) {
            traverser(node, transformation);
        });
    };

    Transformation transformation;
    traverser(this->getRootNode(), transformation);
}

}  // namespace Graphene
