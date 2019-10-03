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
#include <algorithm>
#include <stdexcept>

namespace Graphene {

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
    if (this->skybox != nullptr) {
        this->getPlayer()->detachObject(this->skybox);
    }

    this->skybox = skybox;
    this->getPlayer()->attachObject(this->skybox);
}

std::shared_ptr<Skybox> Scene::getSkybox() const {
    return this->skybox;
}

void Scene::setAmbientColor(const Math::Vec3& ambientColor) {
    this->ambientColor = ambientColor;
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
        modelView = node->getOppositeRotation() * node->getOppositeTranslation() * modelView;
        node = node->getParent();
    }

    modelView = camera->getOppositeRotation() * camera->getOppositeTranslation() * modelView;
    return modelView;
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

Math::Mat4 Scene::calculateTranslation(const std::shared_ptr<Object> object) {
    std::shared_ptr<SceneNode> node = object->getParent();
    Math::Mat4 translation = object->getTranslation();

    while (node != nullptr) {
        // Moving to the root node, current node's transformation matrix is the left operand
        // to be the last operation. Eventually root node's transformation will be the last one.
        translation = node->getTranslation() * translation;
        node = node->getParent();
    }

    return translation;
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

                if (entity->isVisible() && !entity->isSkybox()) {
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
    std::function<void(const std::shared_ptr<SceneNode>, Math::Mat4, Math::Mat4)> traverser;
    traverser = [&handler, &traverser](const std::shared_ptr<SceneNode> node, Math::Mat4 localWorld, Math::Mat4 rotation) {
        // Moving from the root node, current node's transformation matrix is the left operand
        // to be the last operation. Keep the root node's transformation the last one.
        localWorld = localWorld * node->getTranslation() * node->getRotation();
        rotation = rotation * node->getOppositeRotation();

        auto objects = node->getObjects();
        std::for_each(objects.begin(), objects.end(), [&handler, &localWorld, &rotation](const std::shared_ptr<Object> object) {
            if (object->getType() == ObjectType::LIGHT) {
                auto light = std::dynamic_pointer_cast<Light>(object);

                Math::Mat4 lightLocalWorld(localWorld * light->getTranslation() * light->getRotation());
                Math::Mat4 lightRotation(rotation * light->getOppositeRotation());

                Math::Vec3 lightPosition(lightLocalWorld.get(0, 3), lightLocalWorld.get(1, 3), lightLocalWorld.get(2, 3));
                Math::Vec3 lightDirection(lightRotation.get(2, 0), lightRotation.get(2, 1), lightRotation.get(2, 2));

                handler(light, lightPosition, lightDirection);
            }
        });

        auto nodes = node->getNodes();
        std::for_each(nodes.begin(), nodes.end(), [&traverser, &localWorld, &rotation](const std::shared_ptr<SceneNode> node) {
            traverser(node, localWorld, rotation);
        });
    };

    Math::Mat4 localWorld;
    Math::Mat4 rotation;
    traverser(this->rootNode, localWorld, rotation);
}

}  // namespace Graphene
