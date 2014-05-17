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
#include <algorithm>

namespace Graphene {

Light::Light():
        color(1.0f, 1.0f, 1.0f),
        position(Math::Vec3::ZERO),
        direction(Math::Vec3::UNIT_X) {
    this->objectType = ObjectType::TYPE_LIGHT;
    this->lightType = TYPE_POINT;

    LightBuffer light = {
        { 0.0f, 0.0f, 0.0f }, this->energy,
        { 0.0f, 0.0f, 0.0f }, this->falloff,
        { 0.0f, 0.0f, 0.0f }, this->innerAngle,
        this->outerAngle, this->lightType
    };
    std::copy(this->color.data(), this->color.data() + 3, light.color);
    std::copy(this->position.data(), this->position.data() + 3, light.position);
    std::copy(this->direction.data(), this->direction.data() + 3, light.direction);

    this->lightBuffer = std::make_shared<UniformBuffer>(reinterpret_cast<const char*>(&light), sizeof(light));
}

void Light::rotate(const Math::Vec3& vector, float angle) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument("Vector cannot be of zero length");
    }

    Math::Vec3 axis(vector);
    Math::Quaternion q(axis.normalize(), angle * M_PI / 180.0f);
    q.normalize();

    float xAngle, yAngle, zAngle;
    q.extractEulerAngles(xAngle, yAngle, zAngle);

    this->rotationAngles += Math::Vec3(xAngle * 180.f / M_PI, yAngle * 180.f / M_PI, zAngle * 180.f / M_PI);
    this->direction = q.extractMat4().extractMat3() * this->direction;
    updateBuffer(this->lightBuffer, LightBuffer, direction, this->direction.data());
}

}  // namespace Graphene
