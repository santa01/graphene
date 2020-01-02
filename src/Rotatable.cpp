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
    this->rotate(this->getRight(), angle);
}

void Rotatable::yaw(float angle) {
    this->rotate(this->getUp(), angle);
}

void Rotatable::pitch(float angle) {
    this->rotate(this->getTarget(), angle);
}

void Rotatable::rotate(const Math::Vec3& vector, float angle) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument(LogFormat("Vector cannot be of zero length"));
    }

    Math::Vec3 axis(vector);
    axis.normalize();

    float pi = static_cast<float>(M_PI);
    Math::Quaternion q(axis, angle * pi / 180.0f);
    q.normalize();

    float xAngle, yAngle, zAngle;
    q.extractEulerAngles(xAngle, yAngle, zAngle);

    Math::Mat4 rotationMatrix4 = q.extractMat4();
    Math::Mat3 rotationMatrix3 = rotationMatrix4.extractMat3();

    this->rotation = rotationMatrix4 * this->rotation;
    this->rotationAngles += Math::Vec3(xAngle * 180.0f / pi, yAngle * 180.0f / pi, zAngle * 180.0f / pi);

    Math::Vec3 up(this->cameraRotation.get(1, 0), this->cameraRotation.get(1, 1), this->cameraRotation.get(1, 2));
    up = rotationMatrix3 * up;
    up.normalize();

    Math::Vec3 target(this->cameraRotation.get(2, 0), this->cameraRotation.get(2, 1), this->cameraRotation.get(2, 2));
    target = rotationMatrix3 * target;
    target.normalize();

    Math::Vec3 right = up.cross(target);
    right.normalize();

    this->cameraRotation.set(0, 0, right.get(Math::Vec3::X));
    this->cameraRotation.set(0, 1, right.get(Math::Vec3::Y));
    this->cameraRotation.set(0, 2, right.get(Math::Vec3::Z));

    this->cameraRotation.set(1, 0, up.get(Math::Vec3::X));
    this->cameraRotation.set(1, 1, up.get(Math::Vec3::Y));
    this->cameraRotation.set(1, 2, up.get(Math::Vec3::Z));

    this->cameraRotation.set(2, 0, target.get(Math::Vec3::X));
    this->cameraRotation.set(2, 1, target.get(Math::Vec3::Y));
    this->cameraRotation.set(2, 2, target.get(Math::Vec3::Z));
}

Math::Vec3 Rotatable::getRight() const {
    return Math::Vec3(this->cameraRotation.get(0, 0), this->cameraRotation.get(0, 1), this->cameraRotation.get(0, 2));
}

Math::Vec3 Rotatable::getUp() const {
    return Math::Vec3(this->cameraRotation.get(1, 0), this->cameraRotation.get(1, 1), this->cameraRotation.get(1, 2));
}

Math::Vec3 Rotatable::getTarget() const {
    return Math::Vec3(this->cameraRotation.get(2, 0), this->cameraRotation.get(2, 1), this->cameraRotation.get(2, 2));
}

Math::Vec3 Rotatable::getRotationAngles() const {
    return this->rotationAngles;
}

const Math::Mat4& Rotatable::getRotation() const {
    return this->rotation;
}

const Math::Mat4& Rotatable::getCameraRotation() const {
    return this->cameraRotation;
}

}  // namespace Graphene
