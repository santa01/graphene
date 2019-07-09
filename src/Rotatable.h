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

#ifndef ROTATABLE_H
#define ROTATABLE_H

#include <GrapheneApi.h>
#include <Mat4.h>
#include <Vec3.h>

namespace Graphene {

class Rotatable {
public:
    GRAPHENE_API virtual ~Rotatable() = default;

    GRAPHENE_API void roll(float angle);
    GRAPHENE_API void yaw(float angle);
    GRAPHENE_API void pitch(float angle);

    GRAPHENE_API void rotate(const Math::Vec3& vector, float angle);

    GRAPHENE_API Math::Vec3 getRight() const;
    GRAPHENE_API Math::Vec3 getUp() const;
    GRAPHENE_API Math::Vec3 getTarget() const;

    GRAPHENE_API Math::Vec3 getRotationAngles() const;
    GRAPHENE_API const Math::Mat4& getRotation() const;
    GRAPHENE_API const Math::Mat4& getOppositeRotation() const;

private:
    Math::Vec3 updateRotation(Math::Mat4& rotation, const Math::Vec3& axis, float angle);

    Math::Vec3 rotationAngles;
    Math::Mat4 rotation;
    Math::Mat4 oppositeRotation;
};

}  // namespace Graphene

#endif  // ROTATABLE_H
