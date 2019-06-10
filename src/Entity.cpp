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

#include <Entity.h>
#include <cmath>

namespace Graphene {

void Entity::rotate(const Math::Vec3& vector, float angle) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument("Vector cannot be of zero length");
    }

    Math::Vec3 axis(vector);
    float pi = static_cast<float>(M_PI);

    Math::Quaternion q(axis.normalize(), angle * pi / 180.0f);
    q.normalize();

    float xAngle, yAngle, zAngle;
    q.extractEulerAngles(xAngle, yAngle, zAngle);

    this->rotationAngles += Math::Vec3(xAngle * 180.0f / pi, yAngle * 180.0f / pi, zAngle * 180.0f / pi);
    this->rotation = this->rotation * q.extractMat4();
}

}  // namespace Graphene
