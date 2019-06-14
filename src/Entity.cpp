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

#include <Entity.h>
#include <Logger.h>
#include <Quaternion.h>
#include <stdexcept>
#include <algorithm>
#include <cmath>

namespace Graphene {

Entity::Entity() {
    this->objectType = ObjectType::TYPE_ENTITY;
}

void Entity::roll(float angle) {
    this->rotate(Math::Vec3::UNIT_X, angle);
}

void Entity::yaw(float angle) {
    this->rotate(Math::Vec3::UNIT_Y, angle);
}

void Entity::pitch(float angle) {
    this->rotate(Math::Vec3::UNIT_Z, angle);
}

void Entity::rotate(const Math::Vec3& vector, float angle) {
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
    this->rotation = this->rotation * q.extractMat4();
}

Math::Vec3 Entity::getRotationAngles() const {
    return this->rotationAngles;
}

void Entity::scaleX(float factor) {
    this->scale(factor, 1.0f, 1.0f);
}

void Entity::scaleY(float factor) {
    this->scale(1.0f, factor, 1.0f);
}

void Entity::scaleZ(float factor) {
    this->scale(1.0f, 1.0f, factor);
}

void Entity::scale(float xFactor, float yFactor, float zFactor) {
    this->scale(Math::Vec3(xFactor, yFactor, zFactor));
}

void Entity::scale(const Math::Vec3& factors) {
    if (std::any_of(factors.data(), factors.data() + 3, [](float factor) { return (factor <= 0.0f); })) {
        throw std::invalid_argument(LogFormat("Factor is less or equals zero"));
    }

    this->scaling.set(0, 0, this->scaling.get(0, 0) * factors.get(Math::Vec3::X));
    this->scaling.set(1, 1, this->scaling.get(1, 1) * factors.get(Math::Vec3::Y));
    this->scaling.set(2, 2, this->scaling.get(2, 2) * factors.get(Math::Vec3::Z));
}

Math::Vec3 Entity::getScalingFactors() const {
    return Math::Vec3(this->scaling.get(0, 0), this->scaling.get(1, 1), this->scaling.get(2, 2));
}

void Entity::translate(float x, float y, float z) {
    this->translate(Math::Vec3(x, y, z));
}

void Entity::translate(const Math::Vec3& position) {
    this->translation.set(0, 3, position.get(Math::Vec3::X));
    this->translation.set(1, 3, position.get(Math::Vec3::Y));
    this->translation.set(2, 3, position.get(Math::Vec3::Z));
}

void Entity::move(float x, float y, float z) {
    this->move(Math::Vec3(x, y, z));
}

void Entity::move(const Math::Vec3& position) {
    this->translation.set(0, 3, this->translation.get(0, 3) + position.get(Math::Vec3::X));
    this->translation.set(1, 3, this->translation.get(1, 3) + position.get(Math::Vec3::Y));
    this->translation.set(2, 3, this->translation.get(2, 3) + position.get(Math::Vec3::Z));
}

Math::Vec3 Entity::getPosition() const {
    return Math::Vec3(this->translation.get(0, 3), this->translation.get(1, 3), this->translation.get(2, 3));
}

const Math::Mat4& Entity::getTranslation() const {
    return this->translation;
}

const Math::Mat4& Entity::getRotation() const {
    return this->rotation;
}

const Math::Mat4& Entity::getScaling() const {
    return this->scaling;
}

const std::unordered_set<std::shared_ptr<Mesh>>& Entity::getMeshes() {
    return this->meshes;
}

void Entity::addMesh(const std::shared_ptr<Mesh> mesh) {
    if (mesh == nullptr) {
        throw std::invalid_argument(LogFormat("Mesh cannot be nullptr"));
    }

    this->meshes.insert(mesh);
}

}  // namespace Graphene
