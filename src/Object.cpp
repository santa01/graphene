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

#include <Object.h>
#include <ObjectGroup.h>
#include <Logger.h>
#include <stdexcept>
#include <sstream>
#include <cassert>
#include <climits>
#include <cmath>

namespace Graphene {

Object::Object(const std::type_info& objectType):
        objectType(objectType) {
    static int nextObjectId = 0;
    assert(nextObjectId < INT_MAX);
    this->objectId = nextObjectId++;

    std::ostringstream defaultName;
    defaultName << std::hex << this->objectType.name() << " (0x" << this << ")";
    this->objectName = defaultName.str();
}

int Object::getId() const {
    return this->objectId;
}

const std::type_info& Object::getType() const {
    return this->objectType;
}

const std::string& Object::getName() const {
    return this->objectName;
}

void Object::setName(const std::string& objectName) {
    this->objectName = objectName;
}

const std::shared_ptr<Scene> Object::getScene() const {
    auto currentObject = this->shared_from_this();
    auto parentObject = this->getParent();

    while (parentObject != nullptr) {
        currentObject = parentObject;
        parentObject = currentObject->getParent();
    }

    return currentObject->scene.lock();
}

const std::shared_ptr<ObjectGroup> Object::getParent() const {
    return this->parent.lock();
}

void Object::targetAt(float x, float y, float z) {
    this->targetAt(Math::Vec3(x, y, z));
}

void Object::targetAt(const Math::Vec3& vector) {
    if (vector == Math::Vec3::ZERO) {
        throw std::invalid_argument(LogFormat("Vector cannot be of zero length"));
    }

    Math::Vec3 target(this->getTarget());
    Math::Vec3 newTarget(vector - this->getPosition());
    newTarget.normalize();

    float pi = static_cast<float>(M_PI);
    float angle = 180.0f * acosf(target.dot(newTarget)) / pi;

    Math::Vec3 axis = target.cross(newTarget);
    this->rotate(axis, angle);
}

}  // namespace Graphene
