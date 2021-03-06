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

#include <QuadComponent.h>
#include <MetaObject.h>

namespace Graphene {

QuadComponent::QuadComponent(int width, int height):
        Component(QuadComponent::ID),
        width(width),
        height(height) {
}

int QuadComponent::getWidth() const {
    return this->width;
}

void QuadComponent::setWidth(int width) {
    this->width = width;
}

int QuadComponent::getHeight() const {
    return this->height;
}

void QuadComponent::setHeight(int height) {
    this->height = height;
}

void QuadComponent::setOrigin(int x, int y) {
    this->setOrigin(QuadOrigin(x, y));
}

void QuadComponent::setOrigin(const QuadOrigin& origin) {
    this->origin = origin;
}

const QuadOrigin& QuadComponent::getOrigin() const {
    return this->origin;
}

}  // namespace Graphene
