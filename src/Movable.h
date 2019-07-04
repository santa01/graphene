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

#ifndef MOVABLE_H
#define MOVABLE_H

#include <GrapheneApi.h>
#include <Mat4.h>
#include <Vec3.h>

namespace Graphene {

class Movable {
public:
    GRAPHENE_API void translate(float x, float y, float z);
    GRAPHENE_API void translate(const Math::Vec3& position);

    GRAPHENE_API void move(float x, float y, float z);
    GRAPHENE_API void move(const Math::Vec3& position);

    GRAPHENE_API Math::Vec3 getPosition() const;
    GRAPHENE_API const Math::Mat4& getTranslation() const;

protected:
    Math::Mat4 translation;
};

}  // namespace Graphene

#endif  // MOVABLE_H
