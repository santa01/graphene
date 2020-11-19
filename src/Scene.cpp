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
#include <typeinfo>

namespace Graphene {

Scene::Scene() {
    std::ostringstream defaultName;
    defaultName << std::hex << typeid(Scene).name() << " (0x" << this << ")";
    this->sceneName = defaultName.str();
}

const std::shared_ptr<ObjectGroup>& Scene::getRoot() {
    /* shared_from_this() cannot be called from constructor */
    if (this->root == nullptr) {
        this->root = std::make_shared<ObjectGroup>();
        this->root->scene = this->shared_from_this();
    }

    return this->root;
}

const std::shared_ptr<ObjectGroup>& Scene::getPlayer() {
    /* shared_from_this() cannot be called from constructor */
    if (this->player == nullptr) {
        this->player = std::make_shared<ObjectGroup>();
        this->getRoot()->addObject(this->player);
    }

    return this->player;
}

void Scene::setSkybox(const std::shared_ptr<Entity>& skybox) {
    this->skybox = skybox;
}

const std::shared_ptr<Entity>& Scene::getSkybox() const {
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

Math::Mat4 Scene::calculateModelView(const std::shared_ptr<Camera>& camera) {
    auto objectGroup = camera->getParent();
    Math::Mat4 modelView;

    while (objectGroup != nullptr) {
        // Moving to the root object group, current groups's transformation matrix is the left operand
        // to be the last operation. Eventually root object group's transformation will be the last one.
        modelView = objectGroup->getCameraRotation() * objectGroup->getCameraTranslation() * modelView;
        objectGroup = objectGroup->getParent();
    }

    modelView = camera->getCameraRotation() * camera->getCameraTranslation() * modelView;
    return modelView;
}

Math::Mat4 Scene::calculateView(const std::shared_ptr<Camera>& camera) {
    auto objectGroup = camera->getParent();
    Math::Mat4 view;

    while (objectGroup != nullptr) {
        // Moving to the root object group, current group's transformation matrix is the left operand
        // to be the last operation. Eventually root object group's transformation will be the last one.
        view = objectGroup->getCameraRotation() * view;
        objectGroup = objectGroup->getParent();
    }

    view = camera->getCameraRotation() * view;
    return view;
}

Math::Vec3 Scene::calculatePosition(const std::shared_ptr<Camera>& camera) {
    auto objectGroup = camera->getParent();
    Math::Mat4 translation;

    while (objectGroup != nullptr) {
        // Moving to the root object group, current group's transformation matrix is the left operand
        // to be the last operation. Eventually root object group's transformation will be the last one.
        translation = objectGroup->getTranslation() * translation;
        objectGroup = objectGroup->getParent();
    }

    translation = camera->getTranslation() * translation;
    return Math::Vec3(translation.get(0, 3), translation.get(1, 3), translation.get(2, 3));
}

void Scene::iterateEntities(const EntityHandler& handler) const {
    std::function<void(const std::shared_ptr<ObjectGroup>, Math::Mat4, Math::Mat4)> traverser;
    traverser = [&handler, &traverser](const std::shared_ptr<ObjectGroup>& objectGroup, Math::Mat4 localWorld, Math::Mat4 normalRotation) {
        // Moving from the root object group, current group's transformation matrix is the right operand
        // to be the first operation. Keep the root object groups's transformation the last one.
        localWorld = localWorld * objectGroup->getTranslation() * objectGroup->getRotation() * objectGroup->getScaling();
        normalRotation = normalRotation * objectGroup->getRotation();

        auto& objects = objectGroup->getObjects();
        std::for_each(objects.begin(), objects.end(), [&handler, &traverser, &localWorld, &normalRotation](const std::shared_ptr<Object>& object) {
            if (object->isA<Entity>()) {
                auto entity = object->toShared<Entity>();
                if (entity->isVisible()) {
                    Math::Mat4 entityModelView(localWorld * entity->getTranslation() * entity->getRotation() * entity->getScaling());
                    Math::Mat4 entityNormalRotation(normalRotation * entity->getRotation());

                    handler(entity, entityModelView, entityNormalRotation);
                }
            } else if (object->isA<ObjectGroup>()) {
                auto objectGroup = object->toShared<ObjectGroup>();

                traverser(objectGroup, localWorld, normalRotation);
            } 
        });
    };

    Math::Mat4 localWorld;
    Math::Mat4 normalRotation;
    traverser(this->root, localWorld, normalRotation);
}

void Scene::iterateLights(const LightHandler& handler) const {
    std::function<void(const std::shared_ptr<ObjectGroup>, Math::Mat4)> traverser;
    traverser = [&handler, &traverser](const std::shared_ptr<ObjectGroup>& objectGroup, Math::Mat4 localWorld) {
        // Moving from the root object group, current group's transformation matrix is the left operand
        // to be the last operation. Keep the root object group's transformation the last one.
        localWorld = localWorld * objectGroup->getTranslation() * objectGroup->getRotation();

        auto& objects = objectGroup->getObjects();
        std::for_each(objects.begin(), objects.end(), [&handler, &traverser, &localWorld](const std::shared_ptr<Object>& object) {
            if (object->isA<Light>()) {
                auto light = object->toShared<Light>();

                Math::Vec4 lightPosition(localWorld * Math::Vec4(light->getPosition(), 1.0f));
                Math::Vec4 lightDirection(localWorld * Math::Vec4(light->getDirection(), 0.0f));  // Ignore translation
                handler(light, lightPosition.extractVec3(), lightDirection.extractVec3());
            } else if (object->isA<ObjectGroup>()) {
                auto objectGroup = object->toShared<ObjectGroup>();

                traverser(objectGroup, localWorld);
            }
        });
    };

    Math::Mat4 localWorld;
    traverser(this->root, localWorld);
}

}  // namespace Graphene
