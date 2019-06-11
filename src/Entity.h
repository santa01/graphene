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

#ifndef ENTITY_H
#define ENTITY_H

#include <GrapheneApi.h>
#include <Scalable.h>
#include <Mesh.h>
#include <Object.h>
#include <Vec3.h>
#include <Mat4.h>
#include <unordered_set>
#include <memory>

namespace Graphene {

class Entity: public Object, public Scalable {
public:
    GRAPHENE_API Entity();

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

    /* Entity */

    GRAPHENE_API const Math::Mat4& getTranslation() const;
    GRAPHENE_API const Math::Mat4& getRotation() const;
    GRAPHENE_API const Math::Mat4& getScaling() const;

    GRAPHENE_API const std::unordered_set<std::shared_ptr<Mesh>>& getMeshes();
    GRAPHENE_API void addMesh(const std::shared_ptr<Mesh> mesh);

private:
    std::unordered_set<std::shared_ptr<Mesh>> meshes;

    Math::Mat4 translation;
    Math::Mat4 rotation;
    Math::Mat4 scaling;

    Math::Vec3 rotationAngles;
};

}  // namespace Graphene

#endif  // ENTITY_H
