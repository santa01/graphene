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

#include <Object.h>

namespace Graphene {

class Light: public Object {
public:
    Light() {}
    ~Light() {}

    /* Rotatable */

    void roll(float angle) {}
    void yaw(float angle) {}
    void pitch(float angle) {}

    float getXAngle() const {}
    float getYAngle() const {}
    float getZAngle() const {}

    void rotate(const Math::Vec3& vector, float angle) {}

    /* Movable */

    void translate(float x, float y, float z) {}
    void translate(const Math::Vec3& position) {}

    void move(float x, float y, float z) {}
    void move(const Math::Vec3& position) {}

    Math::Vec3 getPosition() const {}

    /* Light */

    // TODO
};

}  // namespace Graphene

#endif  // LIGHT_H
