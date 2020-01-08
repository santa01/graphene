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

#include <Scene.h>
#include <Logger.h>
#include <Object.h>
#include <Vec4.h>
#include <algorithm>
#include <stdexcept>
#include <sstream>

namespace Graphene {

Scene::Scene() {
    std::ostringstream defaultName;
    defaultName << std::hex << this;
    this->sceneName = defaultName.str();
}

std::shared_ptr<SceneNode> Scene::createNode() {
    /* const version of shared_from_this() is selected otherwise */
    return std::make_shared<SceneNode>(this->shared_from_this());
}

std::shared_ptr<SceneNode> Scene::getRootNode() {
    /* shared_from_this() cannot be called from constructor */
    if (this->rootNode == nullptr) {
        this->rootNode = this->createNode();
    }

    return this->rootNode;
}

std::shared_ptr<SceneNode> Scene::getPlayer() {
    /* shared_from_this() cannot be called from constructor */
    if (this->player == nullptr) {
        this->player = this->createNode();
        this->getRootNode()->attachNode(this->player);
    }

    return this->player;
}

void Scene::setSkybox(const std::shared_ptr<Skybox> skybox) {
    this->skybox = skybox;
}

std::shared_ptr<Skybox> Scene::getSkybox() const {
    return this->skybox;
}

const std::string& Scene::getName() const {
    return this->sceneName;
}

void Scene::setName(const std::string& sceneName) {
    this->sceneName = sceneName;
}

void Scene::setAmbientColor(const Math::Vec3& ambientColor) {
    this->ambientColor = ambientColor;
}

void Scene::setAmbientColor(float red, float green, float blue) {
    this->setAmbientColor(Math::Vec3(red, green, blue));
}

const Math::Vec3& Scene::getAmbientColor() const {
    return this->ambientColor;
}

void Scene::setAmbientEnergy(float ambientEnergy) {
    if (ambientEnergy < 0.0f) {
        throw std::runtime_error(LogFormat("Ambient energy is less than 0.0f"));
    }

    this->ambientEnergy = ambientEnergy;
}

float Scene::getAmbientEnergy() const {
    return this->ambientEnergy;
}

Math::Mat4 Scene::calculateModelView(const std::shared_ptr<Camera> camera) {
    std::shared_ptr<SceneNode> node = camera->getParent();
    Math::Mat4 modelView;

    while (node != nullptr) {
        // Moving to the root node, current node's transformation matrix is the left operand
        // to be the last operation. Eventually root node's transformation will be the last one.
        modelView = node->getCameraRotation() * node->getCameraTranslation() * modelView;
        node = node->getParent();
    }

    modelView = camera->getCameraRotation() * camera->getCameraTranslation() * modelView;
    return modelView;
}

Math::Mat4 Scene::calculateView(const std::shared_ptr<Camera> camera) {
    std::shared_ptr<SceneNode> node = camera->getParent();
    Math::Mat4 view;

    while (node != nullptr) {
        // Moving to the root node, current node's transformation matrix is the left operand
        // to be the last operation. Eventually root node's transformation will be the last one.
        view = node->getCameraRotation() * view;
        node = node->getParent();
    }

    view = camera->getCameraRotation() * view;
    return view;
}

Math::Vec3 Scene::calculatePosition(const std::shared_ptr<Camera> camera) {
    std::shared_ptr<SceneNode> node = camera->getParent();
    Math::Mat4 translation;

    while (node != nullptr) {
        // Moving to the root node, current node's transformation matrix is the left operand
        // to be the last operation. Eventually root node's transformation will be the last one.
        translation = node->getTranslation() * translation;
        node = node->getParent();
    }

    translation = camera->getTranslation() * translation;
    return Math::Vec3(translation.get(0, 3), translation.get(1, 3), translation.get(2, 3));
}

void Scene::iterateEntities(EntityHandler handler) {
    std::function<void(const std::shared_ptr<SceneNode>, Math::Mat4, Math::Mat4)> traverser;
    traverser = [&handler, &traverser](const std::shared_ptr<SceneNode> node, Math::Mat4 localWorld, Math::Mat4 normalRotation) {
        // Moving from the root node, current node's transformation matrix is the right operand
        // to be the first operation. Keep the root node's transformation the last one.
        localWorld = localWorld * node->getTranslation() * node->getRotation() * node->getScaling();
        normalRotation = normalRotation * node->getRotation();

        auto objects = node->getObjects();
        std::for_each(objects.begin(), objects.end(), [&handler, &localWorld, &normalRotation](const std::shared_ptr<Object> object) {
            if (object->getType() == ObjectType::ENTITY) {
                auto entity = std::dynamic_pointer_cast<Entity>(object);

                if (entity->isVisible()) {
                    Math::Mat4 entityModelView(localWorld * entity->getTranslation() * entity->getRotation() * entity->getScaling());
                    Math::Mat4 entityNormalRotation(normalRotation * entity->getRotation());

                    handler(entity, entityModelView, entityNormalRotation);
                }
            }
        });

        auto nodes = node->getNodes();
        std::for_each(nodes.begin(), nodes.end(), [&traverser, &localWorld, &normalRotation](const std::shared_ptr<SceneNode> node) {
            traverser(node, localWorld, normalRotation);
        });
    };

    Math::Mat4 localWorld;
    Math::Mat4 normalRotation;
    traverser(this->rootNode, localWorld, normalRotation);
}

void Scene::iterateLights(LightHandler handler) {
    std::function<void(const std::shared_ptr<SceneNode>, Math::Mat4)> traverser;
    traverser = [&handler, &traverser](const std::shared_ptr<SceneNode> node, Math::Mat4 localWorld) {
        // Moving from the root node, current node's transformation matrix is the left operand
        // to be the last operation. Keep the root node's transformation the last one.
        localWorld = localWorld * node->getTranslation() * node->getRotation();

        auto objects = node->getObjects();
        std::for_each(objects.begin(), objects.end(), [&handler, &localWorld](const std::shared_ptr<Object> object) {
            if (object->getType() == ObjectType::LIGHT) {
                auto light = std::dynamic_pointer_cast<Light>(object);

                Math::Vec4 lightPosition(localWorld * Math::Vec4(light->getPosition(), 1.0f));
                Math::Vec4 lightDirection(localWorld * Math::Vec4(light->getDirection(), 0.0f));  // Ignore translation
                handler(light, lightPosition.extractVec3(), lightDirection.extractVec3());
            }
        });

        auto nodes = node->getNodes();
        std::for_each(nodes.begin(), nodes.end(), [&traverser, &localWorld](const std::shared_ptr<SceneNode> node) {
            traverser(node, localWorld);
        });
    };

    Math::Mat4 localWorld;
    traverser(this->rootNode, localWorld);
}

}  // namespace Graphene
