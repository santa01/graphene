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

#ifndef LIGHT_H
#define LIGHT_H

#include <Quaternion.h>
#include <Vec3.h>
#include <Object.h>

namespace Graphene {

class Light: public Object {
public:
    Light() {}
    ~Light() {}

    /* Rotatable */

    void roll(float angle) {
        this->rotate(Math::Vec3::UNIT_X, angle);
    }

    void yaw(float angle) {
        this->rotate(Math::Vec3::UNIT_Y, angle);
    }

    void pitch(float angle) {
        this->rotate(Math::Vec3::UNIT_Z, angle);
    }

    void rotate(const Math::Vec3& vector, float angle) {
        Math::Vec3 axis(vector);
        Math::Quaternion q(axis.normalize(), angle * M_PI / 180.0f);
        q.normalize();

        float xAngle, yAngle, zAngle;
        q.extractEulerAngles(xAngle, yAngle, zAngle);

        this->rotationAngles += Math::Vec3(xAngle * 180.f / M_PI, yAngle * 180.f / M_PI, zAngle * 180.f / M_PI);
        this->direction = q.extractMat4().extractMat3() * this->direction;
    }

    Math::Vec3 getRotationAngles() const {
        return this->rotationAngles;
    }

    /* Movable */

    void translate(float x, float y, float z) {
        this->translate(Math::Vec3(x, y, z));
    }

    void translate(const Math::Vec3& position) {
        this->position = position;
    }

    void move(float x, float y, float z) {
        this->move(Math::Vec3(x, y, z));
    }

    void move(const Math::Vec3& position) {
        this->position += position;
    }

    Math::Vec3 getPosition() const {
        return this->position;
    }

    /* Light */

    // TODO

private:
    Math::Vec3 position;
    Math::Vec3 rotationAngles;
};

}  // namespace Graphene

#endif  // LIGHT_H
