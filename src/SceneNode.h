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

#ifndef SCENENODE_H
#define SCENENODE_H

#include <Rotatable.h>
#include <Scalable.h>
#include <Movable.h>
#include <NonCopyable.h>
#include <Object.h>
#include <unordered_set>
#include <memory>

namespace Graphene {

class SceneManager;

class SceneNode: public std::enable_shared_from_this<SceneNode>,
        public Rotatable, public Scalable, public Movable, public NonCopyable {
public:
    /* Rotatable */

    void roll(float angle) {}
    void yaw(float angle) {}
    void pitch(float angle) {}

    float getXAngle() const {}
    float getYAngle() const {}
    float getZAngle() const {}

    void rotate(const Math::Vec3& vector, float angle) {}

    /* Scalable */

    void scaleX(float factor) {}
    void scaleY(float factor) {}
    void scaleZ(float factor) {}

    float getXFactor() const {}
    float getYFactor() const {}
    float getZFactor() const {}

    void scale(float factor) {}

    /* Movable */

    void translate(float x, float y, float z) {}
    void translate(const Math::Vec3& position) {}

    void move(float x, float y, float z) {}
    void move(const Math::Vec3& position) {}

    Math::Vec3 getPosition() const {}

    /* SceneNode */

    const std::unordered_set<std::shared_ptr<SceneNode>>& getNodes() {
        return this->nodes;
    }

    std::shared_ptr<SceneNode> createNode() {
        // NOTE: SceneNode() is private, std::make_shared will fail
        auto node = std::shared_ptr<SceneNode>(new SceneNode());
        node->sceneManager = this->sceneManager;
        this->nodes.insert(node);
        return node;
    }

    const std::unordered_set<std::shared_ptr<Object>>& getObjects() {
        return this->objects;
    }

    void attachObject(std::shared_ptr<Object> object) {
        auto inserted = this->objects.insert(object);
        if (inserted.second) {
            object->parent = this->shared_from_this();
        }
    }

    std::shared_ptr<class SceneManager> getSceneManager() {
        return this->sceneManager.lock();
    }

    std::shared_ptr<SceneNode> getParent() {
        return this->parent.lock();
    }

private:
    friend class SceneManager;
    SceneNode() = default;

    std::unordered_set<std::shared_ptr<SceneNode>> nodes;
    std::unordered_set<std::shared_ptr<Object>> objects;

    std::weak_ptr<SceneManager> sceneManager;
    std::weak_ptr<SceneNode> parent;
};

}  // namespace Graphene

#endif  // SCENENODE_H