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

SceneNode::SceneNode(const std::shared_ptr<Scene>& scene) {
    if (scene == nullptr) {
        throw std::invalid_argument(LogFormat("Scene cannot be nullptr"));
    }

    this->scene = scene;
}

const std::vector<std::shared_ptr<SceneNode>>& SceneNode::getNodes() const {
    return this->nodes;
}

void SceneNode::attachNode(const std::shared_ptr<SceneNode>& node) {
    if (node == nullptr) {
        throw std::invalid_argument(LogFormat("SceneNode cannot be nullptr"));
    }

    if (!node->parent.expired()) {
        throw std::invalid_argument(LogFormat("SceneNode is already attached elsewhere"));
    }

    node->parent = this->shared_from_this();
    this->nodes.emplace_back(node);
}

const std::vector<std::shared_ptr<Object>>& SceneNode::getObjects() const {
    return this->objects;
}

void SceneNode::attachObject(const std::shared_ptr<Object>& object) {
    if (object == nullptr) {
        throw std::invalid_argument(LogFormat("Object cannot be nullptr"));
    }

    if (!object->parent.expired()) {
        throw std::invalid_argument(LogFormat("Object is already attached elsewhere"));
    }

    object->parent = this->shared_from_this();
    this->objects.emplace_back(object);
}

const std::shared_ptr<Scene> SceneNode::getScene() const {
    return this->scene.lock();
}

const std::shared_ptr<SceneNode> SceneNode::getParent() const {
    return this->parent.lock();
}

}  // namespace Graphene
