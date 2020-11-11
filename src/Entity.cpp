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
#include <Logger.h>
#include <stdexcept>

namespace Graphene {

Entity::Entity():
        Object(ObjectType::ENTITY) {
}

void Entity::setVisible(bool visible) {
    this->visible = visible;
}

bool Entity::isVisible() const {
    return this->visible;
}

const std::vector<std::shared_ptr<Mesh>>& Entity::getMeshes() const {
    return this->meshes;
}

void Entity::addMesh(const std::shared_ptr<Mesh>& mesh) {
    if (mesh == nullptr) {
        throw std::invalid_argument(LogFormat("Mesh cannot be nullptr"));
    }

    this->meshes.emplace_back(mesh);
}

}  // namespace Graphene
