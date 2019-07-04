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

#include <Light.h>
#include <Logger.h>
#include <Quaternion.h>
#include <Mat3.h>
#include <Mat4.h>
#include <algorithm>
#include <stdexcept>
#include <cmath>

namespace Graphene {

Light::Light():
        Object(ObjectType::LIGHT) {
    LightBuffer light = {
        { 0.0f, 0.0f, 0.0f }, this->energy,
        { 0.0f, 0.0f, 0.0f }, this->falloff,
        { 0.0f, 0.0f, 0.0f }, this->angle,
        this->blend, this->lightType
    };
    std::copy(this->color.data(), this->color.data() + 3, light.color);
    std::copy(this->position.data(), this->position.data() + 3, light.position);
    std::copy(this->direction.data(), this->direction.data() + 3, light.direction);

    this->lightBuffer = std::make_shared<UniformBuffer>(&light, sizeof(light));
}

void Light::roll(float angle) {
    this->rotate(Math::Vec3::UNIT_X, angle);
}

void Light::yaw(float angle) {
    this->rotate(Math::Vec3::UNIT_Y, angle);
}

void Light::pitch(float angle) {
    this->rotate(Math::Vec3::UNIT_Z, angle);
}

void Light::rotate(const Math::Vec3& vector, float angle) {
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
    this->direction = q.extractMat4().extractMat3() * this->direction;
    updateBuffer(this->lightBuffer, LightBuffer, direction, this->direction.data());
}

Math::Vec3 Light::getRotationAngles() const {
    return this->rotationAngles;
}

LightType Light::getLightType() const {
    return this->lightType;
}

void Light::setLightType(LightType type) {
    this->lightType = type;
    updateBuffer(this->lightBuffer, LightBuffer, lightType, &this->lightType);
}

const Math::Vec3& Light::getColor() const {
    return this->color;
}

void Light::setColor(float red, float green, float blue) {
    this->setColor(Math::Vec3(red, green, blue));
}

void Light::setColor(const Math::Vec3& color) {
    this->color = color;
    updateBuffer(this->lightBuffer, LightBuffer, color, this->color.data());
}

const Math::Vec3& Light::getDirection() const {
    return this->direction;
}

void Light::setDirection(float x, float y, float z) {
    this->setDirection(Math::Vec3(x, y, z));
}

void Light::setDirection(const Math::Vec3& direction) {
    if (direction == Math::Vec3::ZERO) {
        throw std::invalid_argument(LogFormat("Vector cannot be of zero length"));
    }

    this->direction = direction;
    updateBuffer(this->lightBuffer, LightBuffer, direction, this->direction.data());
}

float Light::getEnergy() const {
    return this->energy;
}

void Light::setEnergy(float energy) {
    this->energy = energy;
    updateBuffer(this->lightBuffer, LightBuffer, energy, &this->energy);
}

float Light::getFalloff() const {
    return this->falloff;
}

void Light::setFalloff(float falloff) {
    this->falloff = falloff;
    updateBuffer(this->lightBuffer, LightBuffer, falloff, &this->falloff);
}

float Light::getAngle() const {
    return this->angle;
}

void Light::setAngle(float angle) {
    this->angle = angle;
    updateBuffer(this->lightBuffer, LightBuffer, angle, &this->angle);
}

float Light::getBlend() const {
    return this->blend;
}

void Light::setBlend(float blend) {
    if (blend < 0.0f || blend > 1.0f) {
        throw std::invalid_argument(LogFormat("Blend is not in [0.0; 1.0] range"));
    }

    this->blend = blend;
    updateBuffer(this->lightBuffer, LightBuffer, blend, &this->blend);
}

std::shared_ptr<UniformBuffer> Light::getLightBuffer() {
    return this->lightBuffer;
}

}  // namespace Graphene
