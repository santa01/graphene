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

#include <GrapheneApi.h>
#include <Rotatable.h>
#include <Scalable.h>
#include <Movable.h>
#include <NonCopyable.h>
#include <Object.h>
#include <Entity.h>
#include <Light.h>
#include <Vec3.h>
#include <unordered_set>
#include <stdexcept>
#include <memory>

namespace Graphene {

class SceneManager;

class SceneNode: public std::enable_shared_from_this<SceneNode>,
        public Rotatable, public Scalable, public Movable, public NonCopyable {
public:
    GRAPHENE_API SceneNode(const std::shared_ptr<SceneManager> sceneManager);

    /* Rotatable */

    GRAPHENE_API void roll(float angle);
    GRAPHENE_API void yaw(float angle);
    GRAPHENE_API void pitch(float angle);

    GRAPHENE_API void rotate(const Math::Vec3& vector, float angle);
    GRAPHENE_API Math::Vec3 getRotationAngles() const;

    /* Scalable */

    GRAPHENE_API void scaleX(float factor);
    GRAPHENE_API void scaleY(float factor);
    GRAPHENE_API void scaleZ(float factor);

    GRAPHENE_API void scale(float xFactor, float yFactor, float zFactor);
    GRAPHENE_API void scale(const Math::Vec3& factors);
    GRAPHENE_API Math::Vec3 getScalingFactors() const;

    /* Movable */

    GRAPHENE_API void translate(float x, float y, float z);
    GRAPHENE_API void translate(const Math::Vec3& position);

    GRAPHENE_API void move(float x, float y, float z);
    GRAPHENE_API void move(const Math::Vec3& position);

    GRAPHENE_API Math::Vec3 getPosition() const;

    /* SceneNode */

    GRAPHENE_API const std::unordered_set<std::shared_ptr<SceneNode>>& getNodes();
    GRAPHENE_API void attachNode(std::shared_ptr<SceneNode> node);

    GRAPHENE_API const std::unordered_set<std::shared_ptr<Object>>& getObjects();
    GRAPHENE_API void attachObject(std::shared_ptr<Object> object);

    GRAPHENE_API std::shared_ptr<class SceneManager> getSceneManager();
    GRAPHENE_API std::shared_ptr<SceneNode> getParent();

private:
    std::unordered_set<std::shared_ptr<SceneNode>> nodes;
    std::unordered_set<std::shared_ptr<Object>> objects;

    std::unordered_set<std::shared_ptr<Entity>> entities;
    std::unordered_set<std::shared_ptr<Light>> lights;

    std::weak_ptr<SceneManager> sceneManager;
    std::weak_ptr<SceneNode> parent;

    Math::Vec3 position;
    Math::Vec3 rotationAngles;
    Math::Vec3 scalingFactors;
};

}  // namespace Graphene

#endif  // SCENENODE_H
