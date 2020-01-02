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

#include <Movable.h>

namespace Graphene {

void Movable::translate(float x, float y, float z) {
    this->translate(Math::Vec3(x, y, z));
}

void Movable::translate(const Math::Vec3& position) {
    this->translation.set(0, 3, position.get(Math::Vec3::X));
    this->translation.set(1, 3, position.get(Math::Vec3::Y));
    this->translation.set(2, 3, position.get(Math::Vec3::Z));

    this->cameraTranslation.set(0, 3, -position.get(Math::Vec3::X));
    this->cameraTranslation.set(1, 3, -position.get(Math::Vec3::Y));
    this->cameraTranslation.set(2, 3, -position.get(Math::Vec3::Z));
}

void Movable::move(float x, float y, float z) {
    this->move(Math::Vec3(x, y, z));
}

void Movable::move(const Math::Vec3& position) {
    this->translate(this->getPosition() + position);
}

Math::Vec3 Movable::getPosition() const {
    return Math::Vec3(this->translation.get(0, 3), this->translation.get(1, 3), this->translation.get(2, 3));
}

const Math::Mat4& Movable::getTranslation() const {
    return this->translation;
}

const Math::Mat4& Movable::getCameraTranslation() const {
    return this->cameraTranslation;
}

}  // namespace Graphene
