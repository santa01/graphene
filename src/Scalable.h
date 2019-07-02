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

#ifndef SCALABLE_H
#define SCALABLE_H

#include <GrapheneApi.h>
#include <Mat4.h>
#include <Vec3.h>

namespace Graphene {

class Scalable {
public:
    GRAPHENE_API void scaleX(float factor);
    GRAPHENE_API void scaleY(float factor);
    GRAPHENE_API void scaleZ(float factor);

    GRAPHENE_API void scale(float xFactor, float yFactor, float zFactor);
    GRAPHENE_API void scale(const Math::Vec3& factors);

    GRAPHENE_API Math::Vec3 getScalingFactors() const;
    GRAPHENE_API const Math::Mat4& getScaling() const;

protected:
    Math::Mat4 scaling;
};

}  // namespace Graphene

#endif  // SCALABLE_H
