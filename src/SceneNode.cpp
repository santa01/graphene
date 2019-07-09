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
#include <Logger.h>
#include <stdexcept>

namespace Graphene {

SceneNode::SceneNode(const std::shared_ptr<SceneManager> sceneManager) {
    if (sceneManager == nullptr) {
        throw std::invalid_argument(LogFormat("SceneManager cannot be nullptr"));
    }

    this->sceneManager = sceneManager;
}

const std::unordered_set<std::shared_ptr<SceneNode>>& SceneNode::getNodes() {
    return this->nodes;
}

void SceneNode::attachNode(std::shared_ptr<SceneNode> node) {
    if (node == nullptr) {
        throw std::invalid_argument(LogFormat("SceneNode cannot be nullptr"));
    }

    auto inserted = this->nodes.insert(node);
    if (inserted.second) {
        node->parent = this->shared_from_this();
    }
}

const std::unordered_set<std::shared_ptr<Object>>& SceneNode::getObjects() {
    return this->objects;
}

void SceneNode::attachObject(std::shared_ptr<Object> object) {
    if (object == nullptr) {
        throw std::invalid_argument(LogFormat("Object cannot be nullptr"));
    }

    auto inserted = this->objects.insert(object);
    if (inserted.second) {
        switch (object->getType()) {
            case ObjectType::ENTITY:
                this->entities.insert(std::dynamic_pointer_cast<Entity>(object));
                break;

            case ObjectType::LIGHT:
                this->lights.insert(std::dynamic_pointer_cast<Light>(object));
                break;

            default:
                break;
        }

        object->parent = this->shared_from_this();
    }
}

std::shared_ptr<class SceneManager> SceneNode::getSceneManager() {
    return this->sceneManager.lock();
}

std::shared_ptr<SceneNode> SceneNode::getParent() {
    return this->parent.lock();
}

}  // namespace Graphene
