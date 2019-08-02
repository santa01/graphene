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

#include <Rotatable.h>
#include <Quaternion.h>
#include <Mat3.h>
#include <Logger.h>
#include <stdexcept>
#include <cmath>

namespace Graphene {

void Rotatable::roll(float angle) {
    float cameraAngle = -angle;

    Math::Vec3 right(this->rotation.get(0, 0), this->rotation.get(0, 1), this->rotation.get(0, 2));
    Math::Vec3 cameraRight(this->oppositeRotation.get(0, 0), this->oppositeRotation.get(0, 1), this->oppositeRotation.get(0, 2));

    this->rotationAngles += this->updateRotation(this->rotation, right, angle);
    this->updateRotation(this->oppositeRotation, cameraRight, cameraAngle);
}

void Rotatable::yaw(float angle) {
    float cameraAngle = -angle;

    Math::Vec3 up(this->rotation.get(1, 0), this->rotation.get(1, 1), this->rotation.get(1, 2));
    Math::Vec3 cameraUp(this->oppositeRotation.get(1, 0), this->oppositeRotation.get(1, 1), this->oppositeRotation.get(1, 2));

    this->rotationAngles += this->updateRotation(this->rotation, up, angle);
    this->updateRotation(this->oppositeRotation, cameraUp, cameraAngle);
}

void Rotatable::pitch(float angle) {
    float cameraAngle = -angle;

    Math::Vec3 target(this->rotation.get(2, 0), this->rotation.get(2, 1), this->rotation.get(2, 2));
    Math::Vec3 cameraTarget(this->oppositeRotation.get(2, 0), this->oppositeRotation.get(2, 1), this->oppositeRotation.get(2, 2));

    this->rotationAngles += this->updateRotation(this->rotation, target, angle);
    this->updateRotation(this->oppositeRotation, cameraTarget, cameraAngle);
}

void Rotatable::rotate(const Math::Vec3& vector, float angle) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument(LogFormat("Vector cannot be of zero length"));
    }

    Math::Vec3 axis(vector);
    axis.normalize();

    float cameraAngle = -angle;

    this->rotationAngles += this->updateRotation(this->rotation, axis, angle);
    this->updateRotation(this->oppositeRotation, axis, cameraAngle);
}

Math::Vec3 Rotatable::getRight() const {
    return Math::Vec3(this->oppositeRotation.get(0, 0), this->oppositeRotation.get(0, 1), this->oppositeRotation.get(0, 2));
}

Math::Vec3 Rotatable::getUp() const {
    return Math::Vec3(this->oppositeRotation.get(1, 0), this->oppositeRotation.get(1, 1), this->oppositeRotation.get(1, 2));
}

Math::Vec3 Rotatable::getTarget() const {
    return Math::Vec3(this->oppositeRotation.get(2, 0), this->oppositeRotation.get(2, 1), this->oppositeRotation.get(2, 2));
}

Math::Vec3 Rotatable::getRotationAngles() const {
    return this->rotationAngles;
}

const Math::Mat4& Rotatable::getRotation() const {
    return this->rotation;
}

const Math::Mat4& Rotatable::getOppositeRotation() const {
    return this->oppositeRotation;
}

Math::Vec3 Rotatable::updateRotation(Math::Mat4& rotation, const Math::Vec3& axis, float angle) {
    // This is a rotation through basis change. Rotate basis vectors into opposite direction
    // to have a real vector rotated once basis is changed to (Right; Up; Target).
    float rotationAngle = -angle;

    float pi = static_cast<float>(M_PI);
    Math::Quaternion q(axis, rotationAngle * pi / 180.0f);
    q.normalize();

    Math::Vec3 up(rotation.get(1, 0), rotation.get(1, 1), rotation.get(1, 2));
    Math::Vec3 target(rotation.get(2, 0), rotation.get(2, 1), rotation.get(2, 2));

    Math::Mat3 rotationMatrix = q.extractMat4().extractMat3();
    up = rotationMatrix * up;
    target = rotationMatrix * target;

    Math::Vec3 right = up.cross(target);
    right.normalize();

    rotation.set(0, 0, right.get(Math::Vec3::X));
    rotation.set(0, 1, right.get(Math::Vec3::Y));
    rotation.set(0, 2, right.get(Math::Vec3::Z));

    rotation.set(1, 0, up.get(Math::Vec3::X));
    rotation.set(1, 1, up.get(Math::Vec3::Y));
    rotation.set(1, 2, up.get(Math::Vec3::Z));

    rotation.set(2, 0, target.get(Math::Vec3::X));
    rotation.set(2, 1, target.get(Math::Vec3::Y));
    rotation.set(2, 2, target.get(Math::Vec3::Z));

    float xAngle, yAngle, zAngle;
    q.extractEulerAngles(xAngle, yAngle, zAngle);

    // Keep initial rotation direction
    return Math::Vec3(-xAngle * 180.0f / pi, -yAngle * 180.0f / pi, -zAngle * 180.0f / pi);
}

}  // namespace Graphene
