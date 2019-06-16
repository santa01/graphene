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

#include <Camera.h>
#include <Logger.h>
#include <Quaternion.h>
#include <Mat3.h>
#include <stdexcept>
#include <cmath>

namespace Graphene {

Camera::Camera() {
    this->objectType = ObjectType::TYPE_CAMERA;

    this->updateProjection(ProjectionType::TYPE_PERSPECTIVE);
    this->rotation.set(2, 2, -1.0f);  // Look at Z
}

void Camera::roll(float angle) {
    this->rotate(Math::Vec3::UNIT_X, angle);
}

void Camera::yaw(float angle) {
    this->rotate(Math::Vec3::UNIT_Y, angle);
}

void Camera::pitch(float angle) {
    this->rotate(Math::Vec3::UNIT_Z, angle);
}

void Camera::rotate(const Math::Vec3& vector, float angle) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument(FormatMessage("Vector cannot be of zero length"));
    }

    Math::Vec3 axis(vector);
    float pi = static_cast<float>(M_PI);

    Math::Quaternion q(axis.normalize(), angle * pi / 180.0f);
    q.normalize();

    Math::Mat3 rotationMatrix = q.extractMat4().extractMat3();
    Math::Vec3 up = this->getUp();
    Math::Vec3 target = -this->getTarget();

    up = rotationMatrix * up;
    target = rotationMatrix * target;

    Math::Vec3 right = target.cross(up);
    right.normalize();

    float xAngle, yAngle, zAngle;
    q.extractEulerAngles(xAngle, yAngle, zAngle);
    this->rotationAngles += Math::Vec3(xAngle * 180.0f / pi, yAngle * 180.0f / pi, zAngle * 180.0f / pi);

    this->updateRotation(right, up, target);
}

Math::Vec3 Camera::getRotationAngles() const {
    return this->rotationAngles;
}

void Camera::translate(float x, float y, float z) {
    this->translate(Math::Vec3(x, y, z));
}

void Camera::translate(const Math::Vec3& position) {
    this->translation.set(0, 3, -position.get(Math::Vec3::X));
    this->translation.set(1, 3, -position.get(Math::Vec3::Y));
    this->translation.set(2, 3, -position.get(Math::Vec3::Z));
}

void Camera::move(float x, float y, float z) {
    this->move(Math::Vec3(x, y, z));
}

void Camera::move(const Math::Vec3& position) {
    this->translation.set(0, 3, this->translation.get(0, 3) - position.get(Math::Vec3::X));
    this->translation.set(1, 3, this->translation.get(1, 3) - position.get(Math::Vec3::Y));
    this->translation.set(2, 3, this->translation.get(2, 3) - position.get(Math::Vec3::Z));
}

Math::Vec3 Camera::getPosition() const {
    return Math::Vec3(-this->translation.get(0, 3), -this->translation.get(1, 3), -this->translation.get(2, 3));
}

Camera::ProjectionType Camera::getProjectionType() const {
    return this->projectionType;
}

void Camera::setProjectionType(ProjectionType type) {
    this->updateProjection(type);
}

float Camera::getAspectRatio() const {
    return this->aspectRatio;
}

void Camera::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    this->updateProjection(this->projectionType);
}

float Camera::getNearPlane() const {
    return this->nearPlane;
}

void Camera::setNearPlane(float nearPlane) {
    this->nearPlane = nearPlane;
    this->updateProjection(this->projectionType);
}

float Camera::getFarPlane() const {
    return this->farPlane;
}

void Camera::setFarPlane(float farPlane) {
    this->farPlane = farPlane;
    this->updateProjection(this->projectionType);
}

float Camera::getFov() const {
    return this->fov;
}

void Camera::setFov(float fov) {
    this->fov = fov;
    this->updateProjection(this->projectionType);
}

Math::Vec3 Camera::getRight() const {
    return Math::Vec3(this->rotation.get(0, 0), this->rotation.get(0, 1), this->rotation.get(0, 2));
}

Math::Vec3 Camera::getUp() const {
    return Math::Vec3(this->rotation.get(1, 0), this->rotation.get(1, 1), this->rotation.get(1, 2));
}

Math::Vec3 Camera::getTarget() const {
    return -Math::Vec3(this->rotation.get(2, 0), this->rotation.get(2, 1), this->rotation.get(2, 2));
}

const Math::Mat4& Camera::getTranslation() const {
    return this->translation;
}

const Math::Mat4& Camera::getRotation() const {
    return this->rotation;
}

const Math::Mat4& Camera::getProjection() const {
    return this->projection;
}

void Camera::lookAt(float x, float y, float z) {
    this->lookAt(Math::Vec3(x, y, z));
}

void Camera::lookAt(const Math::Vec3& vector) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument(FormatMessage("Vector cannot be of zero length"));
    }

    Math::Vec3 target(-vector);
    target.normalize();

    Math::Vec3 right = this->getRight();
    if (target != Math::Vec3::UNIT_Y && target != -Math::Vec3::UNIT_Y) {
        right = target.cross(Math::Vec3::UNIT_Y);
        right.normalize();
    }

    Math::Vec3 up = right.cross(target);
    up.normalize();

    float angle = acosf(target.dot(this->getTarget()));
    Math::Vec3 axis = target.cross(this->getTarget());
    axis.normalize();

    float xAngle = atan2f(axis.get(Math::Vec3::X) * sinf(angle) -
                          axis.get(Math::Vec3::Y) * axis.get(Math::Vec3::Z) * (1 - cosf(angle)),
                          1 - (axis.get(Math::Vec3::X) * axis.get(Math::Vec3::X) +
                               axis.get(Math::Vec3::Z) * axis.get(Math::Vec3::Z)) * (1 - cosf(angle)));
    float yAngle = atan2f(axis.get(Math::Vec3::Y) * sinf(angle) -
                          axis.get(Math::Vec3::X) * axis.get(Math::Vec3::Z) * (1 - cosf(angle)),
                          1 - (axis.get(Math::Vec3::Y) * axis.get(Math::Vec3::Y) +
                               axis.get(Math::Vec3::Z) * axis.get(Math::Vec3::Z)) * (1 - cosf(angle)));
    float zAngle = asinf(axis.get(Math::Vec3::X) * axis.get(Math::Vec3::Y) * (1 - cosf(angle)) +
                         axis.get(Math::Vec3::Z) * sinf(angle));

    float pi = static_cast<float>(M_PI);
    this->rotationAngles += Math::Vec3(xAngle * 180.0f / pi, yAngle * 180.0f / pi, zAngle * 180.0f / pi);
    this->updateRotation(right, up, target);
}

void Camera::updateProjection(ProjectionType projectionType) {
    this->projection = Math::Mat4();
    this->projectionType = projectionType;
    float pi = static_cast<float>(M_PI);

    switch (this->projectionType) {
        case ProjectionType::TYPE_PERSPECTIVE:
            this->projection.set(0, 0, 1.0f / (tanf(this->fov * pi / 180.0f / 2.0f) *
                                       this->aspectRatio));
            this->projection.set(1, 1, 1.0f / (tanf(this->fov * pi / 180.0f / 2.0f)));
            this->projection.set(2, 2, (-this->farPlane - this->nearPlane) /
                                       (this->farPlane - this->nearPlane));
            this->projection.set(2, 3, (-2.0f * this->farPlane * this->nearPlane) /
                                       (this->farPlane - this->nearPlane));
            this->projection.set(3, 2, -1.0f);
            this->projection.set(3, 3, 0.0f);
            break;
        case ProjectionType::TYPE_ORTHOGRAPHIC:
            this->projection.set(0, 0, 1.0f / ((this->farPlane - this->nearPlane) *
                                       this->aspectRatio));
            this->projection.set(1, 1, 1.0f / (this->farPlane - this->nearPlane));
            this->projection.set(2, 2, -2.0f / (this->farPlane - this->nearPlane));
            this->projection.set(2, 3, (-this->farPlane - this->nearPlane) /
                                       (this->farPlane - this->nearPlane));
            break;
    }
}

void Camera::updateRotation(const Math::Vec3& right, const Math::Vec3& up, const Math::Vec3& target) {
    this->rotation.set(0, 0, right.get(Math::Vec3::X));
    this->rotation.set(0, 1, right.get(Math::Vec3::Y));
    this->rotation.set(0, 2, right.get(Math::Vec3::Z));

    this->rotation.set(1, 0, up.get(Math::Vec3::X));
    this->rotation.set(1, 1, up.get(Math::Vec3::Y));
    this->rotation.set(1, 2, up.get(Math::Vec3::Z));

    this->rotation.set(2, 0, target.get(Math::Vec3::X));
    this->rotation.set(2, 1, target.get(Math::Vec3::Y));
    this->rotation.set(2, 2, target.get(Math::Vec3::Z));
}

}  // namespace Graphene
