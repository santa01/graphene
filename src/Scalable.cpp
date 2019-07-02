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

#include <Scalable.h>
#include <Logger.h>
#include <stdexcept>

namespace Graphene {

void Scalable::scaleX(float factor) {
    this->scale(factor, 1.0f, 1.0f);
}

void Scalable::scaleY(float factor) {
    this->scale(1.0f, factor, 1.0f);
}

void Scalable::scaleZ(float factor) {
    this->scale(1.0f, 1.0f, factor);
}

void Scalable::scale(float xFactor, float yFactor, float zFactor) {
    this->scale(Math::Vec3(xFactor, yFactor, zFactor));
}

void Scalable::scale(const Math::Vec3& factors) {
    float factorX = factors.get(Math::Vec3::X);
    float factorY = factors.get(Math::Vec3::Y);
    float factorZ = factors.get(Math::Vec3::Z);

    if (factorX <= 0.0f || factorY <= 0.0f || factorZ <= 0.0f) {
        throw std::invalid_argument(LogFormat("Factor is less or equals zero"));
    }

    this->scaling.set(0, 0, this->scaling.get(0, 0) * factorX);
    this->scaling.set(1, 1, this->scaling.get(1, 1) * factorY);
    this->scaling.set(2, 2, this->scaling.get(2, 2) * factorZ);
}

Math::Vec3 Scalable::getScalingFactors() const {
    return Math::Vec3(this->scaling.get(0, 0), this->scaling.get(1, 1), this->scaling.get(2, 2));
}

const Math::Mat4& Scalable::getScaling() const {
    return this->scaling;
}

}  // namespace Graphene
