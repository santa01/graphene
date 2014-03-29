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

#include <SceneNode.h>
#include <Quaternion.h>
#include <Mat3.h>

namespace Graphene {

void SceneNode::rotate(const Math::Vec3& vector, float angle) {
    Math::Vec3 axis(vector);
    Math::Quaternion q(axis.normalize(), angle * M_PI / 180.0f);
    float xAngle, yAngle, zAngle;

    q.normalize().extractEulerAngles(xAngle, yAngle, zAngle);
    this->rotationAngles += Math::Vec3(xAngle * 180.f / M_PI, yAngle * 180.f / M_PI, zAngle * 180.f / M_PI);

    for (auto& node: this->nodes) {
        node->rotate(vector, angle);
    }

    for (auto& object: this->objects) {
        object->rotate(vector, angle);
    }
}


void SceneNode::scale(const Math::Vec3& factors) {
    Math::Mat3 scalingMatrix;
    scalingMatrix.set(0, 0, factors.get(Math::Vec3::X));
    scalingMatrix.set(1, 1, factors.get(Math::Vec3::Y));
    scalingMatrix.set(2, 2, factors.get(Math::Vec3::Z));

    this->scalingFactors = scalingMatrix * this->scalingFactors;

    for (auto& node: this->nodes) {
        node->translate(scalingMatrix * node->getPosition());
        node->scale(factors);
    }

    for (auto& entity: this->entities) {
        entity->translate(scalingMatrix * entity->getPosition());
        entity->scale(factors);
    }
}

}  // namespace Graphene
