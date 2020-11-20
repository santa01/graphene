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

#ifndef CAMERA_H
#define CAMERA_H

#include <GrapheneApi.h>
#include <MetaObject.h>
#include <Object.h>
#include <Mat4.h>
#include <Vec3.h>

namespace Graphene {

enum class ProjectionType { PERSPECTIVE, ORTHOGRAPHIC };

class Camera: public MetaObject<Camera>, public Object {
public:
    GRAPHENE_API Camera(ProjectionType type);

    GRAPHENE_API ProjectionType getProjectionType() const;
    GRAPHENE_API void setProjectionType(ProjectionType type);

    GRAPHENE_API float getAspectRatio() const;
    GRAPHENE_API void setAspectRatio(float aspectRatio);

    GRAPHENE_API float getNearPlane() const;
    GRAPHENE_API void setNearPlane(float nearPlane);

    GRAPHENE_API float getFarPlane() const;
    GRAPHENE_API void setFarPlane(float farPlane);

    GRAPHENE_API float getFov() const;
    GRAPHENE_API void setFov(float fov);

    GRAPHENE_API const Math::Mat4& getProjection() const;

    GRAPHENE_API void lookAt(float x, float y, float z);
    GRAPHENE_API void lookAt(const Math::Vec3& target);

private:
    void updateProjection();

    ProjectionType projectionType = ProjectionType::PERSPECTIVE;
    Math::Mat4 projection;

    float aspectRatio = 1.3333f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    float fov = 70.0f;
};

}  // namespace Graphene

#endif  // CAMERA_H
