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
#include <stdexcept>

namespace Graphene {

class Light: public Object {
public:
    enum LightType {
        TYPE_POINT,
        TYPE_SPOT,
        TYPE_DIRECTED
    };

    Light():
            direction(Math::Vec3::UNIT_X) {
        this->objectType = ObjectType::TYPE_LIGHT;
        this->lightType = TYPE_POINT;
    }

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

    LightType getLightType() const {
        return this->lightType;
    }

    void setLightType(LightType type) {
        this->lightType = type;
    }

    void setColor(float red, float green, float blue) {
        this->setColor(Math::Vec3(red, green, blue));
    }

    void setColor(const Math::Vec3& color) {
        this->color = color;
    }

    const Math::Vec3& getColor() const {
        return this->color;
    }

    void setDirection(float x, float y, float z) {
        this->setDirection(Math::Vec3(x, y, z));
    }

    void setDirection(const Math::Vec3& direction) {
        if (direction == Math::Vec3::ZERO) {
            throw std::invalid_argument("Vector cannot be of zero length");
        }

        this->direction = direction;
    }

    const Math::Vec3& getDirection() const {
        return this->direction;
    }

    void setEnergy(float energy) {
        this->energy = energy;
    }

    float getEnergy() const {
        return this->energy;
    }

    void setFalloff(float falloff) {
        this->falloff = falloff;
    }

    float getFalloff() const {
        return this->falloff;
    }

    void setInnerAngle(float angle) {
        if (angle < 0.0f || angle > this->outerAngle) {
            throw std::invalid_argument("Angle is not in [0.0; outerAngle] range");
        }

        this->innerAngle = angle;
    }

    float getInnerAngle() const {
        return this->innerAngle;
    }

    void setOuterAngle(float angle) {
        if (angle < 0.0f || angle >= 180.0f) {
            throw std::invalid_argument("Angle is not in [0.0; 180.0) range");
        }

        this->outerAngle = angle;
    }

    float getOuterAngle() const {
        return this->outerAngle;
    }

private:
    LightType lightType;

    Math::Vec3 color;
    Math::Vec3 position;   // TYPE_POINT, TYPE_SPOT
    Math::Vec3 direction;  // TYPE_SPOT, TYPE_DIRECTED

    float energy;
    float falloff;
    float innerAngle;      // TYPE_SPOT
    float outerAngle;      // TYPE_SPOT

    Math::Vec3 rotationAngles;
};

}  // namespace Graphene

#endif  // LIGHT_H
