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
#include <Quaternion.h>
#include <Mat3.h>
#include <stdexcept>
#include <cmath>

namespace Graphene {

void Camera::rotate(const Math::Vec3& vector, float angle) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument("Vector cannot be of zero length");
    }

    Math::Vec3 axis(vector);
    Math::Quaternion q(axis.normalize(), angle * M_PI / 180.0f);
    q.normalize();

    Math::Mat3 rotationMatrix = q.extractMat4().extractMat3();
    Math::Vec3 up = this->getUp();
    Math::Vec3 target = this->getTarget();

    up = rotationMatrix * up;
    target = rotationMatrix * target;

    Math::Vec3 right = target.cross(up);
    right.normalize();

    float xAngle, yAngle, zAngle;
    q.extractEulerAngles(xAngle, yAngle, zAngle);
    this->rotationAngles += Math::Vec3(xAngle * 180.f / M_PI, yAngle * 180.f / M_PI, zAngle * 180.f / M_PI);

    this->updateRotation(right, up, target);
}

void Camera::lookAt(const Math::Vec3& vector) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument("Vector cannot be of zero length");
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

    this->updateRotation(right, up, target);
}

void Camera::updateProjection(ProjectionType projectionType) {
    this->projection = Math::Mat4();
    this->projectionType = projectionType;

    switch (this->projectionType) {
        case TYPE_PERSPECTIVE:
            this->projection.set(0, 0, 1.0f / (tanf(this->fov * M_PI / 180.0f / 2.0f) *
                                       this->aspectRatio));
            this->projection.set(1, 1, 1.0f / (tanf(this->fov * M_PI / 180.0f / 2.0f)));
            this->projection.set(2, 2, (-this->farPlane - this->nearPlane) /
                                       (this->farPlane - this->nearPlane));
            this->projection.set(2, 3, (-2.0f * this->farPlane * this->nearPlane) /
                                       (this->farPlane - this->nearPlane));
            this->projection.set(3, 2, -1.0f);
            this->projection.set(3, 3, 0.0f);
            break;
        case TYPE_ORTHOGRAPHIC:
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
