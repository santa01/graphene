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
#include <stdexcept>
#include <cmath>

namespace Graphene {

#pragma pack(push, 1)

/* std140 layout */
typedef struct {
    float color[3];
    int type;
    float energy;
    float falloff;
    float angle;
    float blend;
} LightBuffer;

#pragma pack(pop)

Light::Light(LightType lightType):
        Object(typeid(Light)),
        lightType(lightType) {
    LightBuffer buffer = { };
    this->lightBuffer = std::make_shared<UniformBuffer>(&buffer, sizeof(buffer));
}

LightType Light::getLightType() const {
    return this->lightType;
}

void Light::setLightType(LightType lightType) {
    this->lightType = lightType;
    this->parametersDirty = true;
}

float Light::getEnergy() const {
    return this->energy;
}

void Light::setEnergy(float energy) {
    this->energy = energy;
    this->parametersDirty = true;
}

float Light::getFalloff() const {
    return this->falloff;
}

void Light::setFalloff(float falloff) {
    this->falloff = falloff;
    this->parametersDirty = true;
}

float Light::getAngle() const {
    return this->angle;
}

void Light::setAngle(float angle) {
    this->angle = angle;
    this->parametersDirty = true;
}

float Light::getBlend() const {
    return this->blend;
}

void Light::setBlend(float blend) {
    if (blend < 0.0f || blend > 1.0f) {
        throw std::invalid_argument(LogFormat("Blend is not in [0.0; 1.0] range"));
    }

    this->blend = blend;
    this->parametersDirty = true;
}

const Math::Vec3& Light::getColor() const {
    return this->color;
}

void Light::setColor(float red, float green, float blue) {
    this->setColor(Math::Vec3(red, green, blue));
}

void Light::setColor(const Math::Vec3& color) {
    this->color = color;
    this->parametersDirty = true;
}

const Math::Vec3 Light::getDirection() const {
    return this->getTarget();
}

void Light::setDirection(float x, float y, float z) {
    this->setDirection(Math::Vec3(x, y, z));
}

void Light::setDirection(const Math::Vec3& direction) {
    this->targetAt(direction);
}

void Light::bind(BindPoint bindPoint) {
    if (this->parametersDirty) {
        this->parametersDirty = false;
        this->updateLightBuffer();
    }

    this->lightBuffer->bind(bindPoint);
}

void Light::updateLightBuffer() {
    LightBuffer buffer = { };

    std::copy(this->color.data(), this->color.data() + 3, buffer.color);

    buffer.type = this->lightType;
    buffer.energy = this->energy;
    buffer.falloff = this->falloff;
    buffer.angle = this->angle;
    buffer.blend = this->blend;

    this->lightBuffer->update(&buffer, sizeof(buffer));
}

}  // namespace Graphene
