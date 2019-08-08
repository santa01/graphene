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

#ifndef SCENE_H
#define SCENE_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <SceneNode.h>
#include <Camera.h>
#include <Entity.h>
#include <Light.h>
#include <Mat4.h>
#include <Vec3.h>
#include <functional>
#include <memory>

namespace Graphene {

typedef std::function<void(const std::shared_ptr<Entity>, const Math::Mat4&, const Math::Mat4&)> EntityHandler;
typedef std::function<void(const std::shared_ptr<Light>, const Math::Vec3&, const Math::Vec3&)> LightHandler;

class Scene: public std::enable_shared_from_this<Scene>, public NonCopyable {
public:
    GRAPHENE_API std::shared_ptr<SceneNode> createNode();
    GRAPHENE_API std::shared_ptr<SceneNode> getRootNode();

    GRAPHENE_API void setAmbientColor(const Math::Vec3& ambientColor);
    GRAPHENE_API const Math::Vec3& getAmbientColor() const;

    GRAPHENE_API void setAmbientEnergy(float ambientEnergy);
    GRAPHENE_API float getAmbientEnergy() const;

    GRAPHENE_API static Math::Mat4 calculateModelView(const std::shared_ptr<Camera> camera);
    GRAPHENE_API void iterateEntities(EntityHandler handler);
    GRAPHENE_API void iterateLights(LightHandler handler);

private:
    std::shared_ptr<SceneNode> rootNode;

    Math::Vec3 ambientColor = { 1.0f, 1.0f, 1.0f };
    float ambientEnergy = 1.0f;
};

}  // namespace Graphene

#endif  // SCENE_H
