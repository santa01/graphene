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
#include <Mat3.h>
#include <stdexcept>
#include <cmath>

namespace Graphene {

Camera::Camera():
        Object(ObjectType::CAMERA) {
    this->updateProjection(ProjectionType::PERSPECTIVE);
    this->rotation.set(2, 2, -1.0f);  // Look at Z
}

ProjectionType Camera::getProjectionType() const {
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

const Math::Mat4& Camera::getProjection() const {
    return this->projection;
}

void Camera::lookAt(float x, float y, float z) {
    this->lookAt(Math::Vec3(x, y, z));
}

void Camera::lookAt(const Math::Vec3& vector) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument(LogFormat("Vector cannot be of zero length"));
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
        case ProjectionType::PERSPECTIVE:
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
        case ProjectionType::ORTHOGRAPHIC:
            this->projection.set(0, 0, 1.0f / ((this->farPlane - this->nearPlane) *
                                       this->aspectRatio));
            this->projection.set(1, 1, 1.0f / (this->farPlane - this->nearPlane));
            this->projection.set(2, 2, -2.0f / (this->farPlane - this->nearPlane));
            this->projection.set(2, 3, (-this->farPlane - this->nearPlane) /
                                       (this->farPlane - this->nearPlane));
            break;
    }
}

}  // namespace Graphene
