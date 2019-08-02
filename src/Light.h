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

#include <GrapheneApi.h>
#include <UniformBuffer.h>
#include <Object.h>
#include <Vec3.h>
#include <memory>

namespace Graphene {

enum LightType { POINT, SPOT, DIRECTED };

class Light: public Object {
public:
    GRAPHENE_API Light(LightType type);

    GRAPHENE_API LightType getLightType() const;
    GRAPHENE_API void setLightType(LightType type);

    GRAPHENE_API float getEnergy() const;
    GRAPHENE_API void setEnergy(float energy);

    GRAPHENE_API float getFalloff() const;
    GRAPHENE_API void setFalloff(float falloff);

    GRAPHENE_API float getAngle() const;
    GRAPHENE_API void setAngle(float angle);

    GRAPHENE_API float getBlend() const;
    GRAPHENE_API void setBlend(float blend);

    GRAPHENE_API const Math::Vec3& getColor() const;
    GRAPHENE_API void setColor(float red, float green, float blue);
    GRAPHENE_API void setColor(const Math::Vec3& color);

    GRAPHENE_API const Math::Vec3 getDirection() const;
    GRAPHENE_API void setDirection(float x, float y, float z);
    GRAPHENE_API void setDirection(const Math::Vec3& direction);

    GRAPHENE_API std::shared_ptr<UniformBuffer> getLightBuffer() const;

private:
    std::shared_ptr<UniformBuffer> lightBuffer;

    LightType type = LightType::POINT;
    float energy = 1.0f;
    float falloff = 25.0f;
    float angle = 45.0f;  // TYPE_SPOT
    float blend = 0.15f;  // TYPE_SPOT

    Math::Vec3 color = { 1.0f, 1.0f, 1.0f };
};

}  // namespace Graphene

#endif  // LIGHT_H
