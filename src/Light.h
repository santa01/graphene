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

#include <UniformBuffer.h>
#include <Quaternion.h>
#include <Vec3.h>
#include <Object.h>
#include <stdexcept>
#include <memory>

namespace Graphene {

#pragma pack(push, 1)

/* std140 layout */
typedef struct {
    float color[3];
    float energy;
    float position[3];
    float falloff;
    float direction[3];
    float angle;
    float blend;
    int lightType;
} LightBuffer;

#pragma pack(pop)

class Light: public Object {
public:
    enum LightType {
        TYPE_POINT,
        TYPE_SPOT,
        TYPE_DIRECTED
    };

    Light();

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

    void rotate(const Math::Vec3& vector, float angle);

    Math::Vec3 getRotationAngles() const {
        return this->rotationAngles;
    }

    /* Movable */

    void translate(float x, float y, float z) {
        this->translate(Math::Vec3(x, y, z));
    }

    void translate(const Math::Vec3& position) {
        this->position = position;
        updateBuffer(this->lightBuffer, LightBuffer, position, this->position.data());
    }

    void move(float x, float y, float z) {
        this->move(Math::Vec3(x, y, z));
    }

    void move(const Math::Vec3& position) {
        this->position += position;
        updateBuffer(this->lightBuffer, LightBuffer, position, this->position.data());
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
        updateBuffer(this->lightBuffer, LightBuffer, lightType, &this->lightType);
    }

    const Math::Vec3& getColor() const {
        return this->color;
    }

    void setColor(float red, float green, float blue) {
        this->setColor(Math::Vec3(red, green, blue));
    }

    void setColor(const Math::Vec3& color) {
        this->color = color;
        updateBuffer(this->lightBuffer, LightBuffer, color, this->color.data());
    }

    const Math::Vec3& getDirection() const {
        return this->direction;
    }

    void setDirection(float x, float y, float z) {
        this->setDirection(Math::Vec3(x, y, z));
    }

    void setDirection(const Math::Vec3& direction) {
        if (direction == Math::Vec3::ZERO) {
            throw std::invalid_argument("Vector cannot be of zero length");
        }

        this->direction = direction;
        updateBuffer(this->lightBuffer, LightBuffer, direction, this->direction.data());
    }

    float getEnergy() const {
        return this->energy;
    }

    void setEnergy(float energy) {
        this->energy = energy;
        updateBuffer(this->lightBuffer, LightBuffer, energy, &this->energy);
    }

    float getFalloff() const {
        return this->falloff;
    }

    void setFalloff(float falloff) {
        this->falloff = falloff;
        updateBuffer(this->lightBuffer, LightBuffer, falloff, &this->falloff);
    }

    float getAngle() const {
        return this->angle;
    }

    void setAngle(float angle) {
        this->angle = angle;
        updateBuffer(this->lightBuffer, LightBuffer, angle, &this->angle);
    }

    float getBlend() const {
        return this->blend;
    }

    void setBlend(float blend) {
        if (blend < 0.0f || blend > 1.0f) {
            throw std::invalid_argument("Blend is not in [0.0; 1.0] range");
        }

        this->blend = blend;
        updateBuffer(this->lightBuffer, LightBuffer, blend, &this->blend);
    }

    std::shared_ptr<UniformBuffer> getLightBuffer() {
        return this->lightBuffer;
    }

private:
    LightType lightType;
    std::shared_ptr<UniformBuffer> lightBuffer;

    Math::Vec3 color;
    Math::Vec3 position;   // TYPE_POINT, TYPE_SPOT
    Math::Vec3 direction;  // TYPE_SPOT, TYPE_DIRECTED

    float energy;
    float falloff;
    float angle;  // TYPE_SPOT
    float blend;  // TYPE_SPOT

    Math::Vec3 rotationAngles;
};

}  // namespace Graphene

#endif  // LIGHT_H
