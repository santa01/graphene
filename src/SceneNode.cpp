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

#include <SceneNode.h>
#include <Logger.h>
#include <Quaternion.h>
#include <Mat3.h>
#include <algorithm>
#include <stdexcept>
#include <cmath>

namespace Graphene {

SceneNode::SceneNode(const std::shared_ptr<SceneManager> sceneManager):
        scalingFactors(1.0f, 1.0f, 1.0f) {
    if (sceneManager == nullptr) {
        throw std::invalid_argument(LogFormat("SceneManager cannot be nullptr"));
    }

    this->sceneManager = sceneManager;
}

void SceneNode::roll(float angle) {
    this->rotate(Math::Vec3::UNIT_X, angle);
}

void SceneNode::yaw(float angle) {
    this->rotate(Math::Vec3::UNIT_Y, angle);
}

void SceneNode::pitch(float angle) {
    this->rotate(Math::Vec3::UNIT_Z, angle);
}

void SceneNode::rotate(const Math::Vec3& vector, float angle) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument(LogFormat("Vector cannot be of zero length"));
    }

    Math::Vec3 axis(vector);
    float pi = static_cast<float>(M_PI);

    Math::Quaternion q(axis.normalize(), angle * pi / 180.0f);
    q.normalize();

    float xAngle, yAngle, zAngle;
    q.extractEulerAngles(xAngle, yAngle, zAngle);
    this->rotationAngles += Math::Vec3(xAngle * 180.0f / pi, yAngle * 180.0f / pi, zAngle * 180.0f / pi);

    Math::Vec3 position = this->getPosition();
    Math::Mat3 rotationMatrix = q.extractMat4().extractMat3();

    for (auto& node: this->nodes) {
        node->translate(position + rotationMatrix * (node->getPosition() - position));
        node->rotate(vector, angle);
    }

    for (auto& object: this->objects) {
        object->translate(position + rotationMatrix * (object->getPosition() - position));
        object->rotate(vector, angle);
    }
}

Math::Vec3 SceneNode::getRotationAngles() const {
    return this->rotationAngles;
}

void SceneNode::scaleX(float factor) {
    this->scale(factor, 1.0f, 1.0f);
}

void SceneNode::scaleY(float factor) {
    this->scale(1.0f, factor, 1.0f);
}

void SceneNode::scaleZ(float factor) {
    this->scale(1.0f, 1.0f, factor);
}

void SceneNode::scale(float xFactor, float yFactor, float zFactor) {
    this->scale(Math::Vec3(xFactor, yFactor, zFactor));
}

void SceneNode::scale(const Math::Vec3& factors) {
    if (std::any_of(factors.data(), factors.data() + 3, [](float factor) { return (factor <= 0.0f); })) {
        throw std::invalid_argument(LogFormat("Factor is less or equals zero"));
    }

    Math::Mat3 scalingMatrix;
    scalingMatrix.set(0, 0, factors.get(Math::Vec3::X));
    scalingMatrix.set(1, 1, factors.get(Math::Vec3::Y));
    scalingMatrix.set(2, 2, factors.get(Math::Vec3::Z));
    this->scalingFactors = scalingMatrix * this->scalingFactors;

    Math::Vec3 position = this->getPosition();

    for (auto& node: this->nodes) {
        node->translate(position + scalingMatrix * (node->getPosition() - position));
        node->scale(factors);
    }

    for (auto& entity: this->entities) {
        entity->translate(position + scalingMatrix * (entity->getPosition() - position));
        entity->scale(factors);
    }
}

Math::Vec3 SceneNode::getScalingFactors() const {
    return this->scalingFactors;
}

void SceneNode::translate(float x, float y, float z) {
    this->translate(Math::Vec3(x, y, z));
}

void SceneNode::translate(const Math::Vec3& position) {
    this->move(position - this->position);
}

void SceneNode::move(float x, float y, float z) {
    this->move(Math::Vec3(x, y, z));
}

void SceneNode::move(const Math::Vec3& position) {
    this->position += position;

    for (auto& node: this->nodes) {
        node->move(position);
    }

    for (auto& object: this->objects) {
        object->move(position);
    }
}

Math::Vec3 SceneNode::getPosition() const {
    return this->position;
}

const std::unordered_set<std::shared_ptr<SceneNode>>& SceneNode::getNodes() {
    return this->nodes;
}

void SceneNode::attachNode(std::shared_ptr<SceneNode> node) {
    if (node == nullptr) {
        throw std::invalid_argument(LogFormat("SceneNode cannot be nullptr"));
    }

    auto inserted = this->nodes.insert(node);
    if (inserted.second) {
        node->parent = this->shared_from_this();
    }
}

const std::unordered_set<std::shared_ptr<Object>>& SceneNode::getObjects() {
    return this->objects;
}

void SceneNode::attachObject(std::shared_ptr<Object> object) {
    if (object == nullptr) {
        throw std::invalid_argument(LogFormat("Object cannot be nullptr"));
    }

    auto inserted = this->objects.insert(object);
    if (inserted.second) {
        switch (object->getObjectType()) {
            case Object::ObjectType::TYPE_ENTITY:
                this->entities.insert(std::dynamic_pointer_cast<Entity>(object));
                break;

            case Object::ObjectType::TYPE_LIGHT:
                this->lights.insert(std::dynamic_pointer_cast<Light>(object));
                break;

            default:
                break;
        }

        object->parent = this->shared_from_this();
    }
}

std::shared_ptr<class SceneManager> SceneNode::getSceneManager() {
    return this->sceneManager.lock();
}

std::shared_ptr<SceneNode> SceneNode::getParent() {
    return this->parent.lock();
}

}  // namespace Graphene
