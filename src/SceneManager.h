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

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <SceneNode.h>
#include <Camera.h>
#include <Shader.h>
#include <Mesh.h>
#include <Vec3.h>
#include <algorithm>
#include <functional>
#include <memory>

namespace Graphene {

#pragma pack(push, 1)

typedef struct {
    float positons[12];
    float normals[12];
    float uvs[8];
    int faces[6];
} FrameGeometry;

#pragma pack(pop)

class SceneManager: public std::enable_shared_from_this<SceneManager>, public NonCopyable {
public:
    GRAPHENE_API SceneManager();

    GRAPHENE_API std::shared_ptr<SceneNode> createNode();
    GRAPHENE_API std::shared_ptr<SceneNode> getRootNode();

    GRAPHENE_API std::shared_ptr<Shader> getGeometryShader();
    GRAPHENE_API void setGeometryShader(const std::shared_ptr<Shader> geometryShader);

    GRAPHENE_API std::shared_ptr<Shader> getAmbientShader();
    GRAPHENE_API void setAmbientShader(const std::shared_ptr<Shader> ambientShader);

    GRAPHENE_API std::shared_ptr<Shader> getLightingShader();
    GRAPHENE_API void setLightingShader(const std::shared_ptr<Shader> lightingShader);

    GRAPHENE_API const Math::Vec3& getAmbientColor() const;
    GRAPHENE_API void setAmbientColor(const Math::Vec3& ambientColor);

    GRAPHENE_API float getAmbientEnergy() const;
    GRAPHENE_API void setAmbientEnergy(float ambientEnergy);

    GRAPHENE_API bool hasShadowPass() const;
    GRAPHENE_API void setShadowPass(bool shadowPass);

    GRAPHENE_API bool hasLightPass() const;
    GRAPHENE_API void setLightPass(bool lightPass);

    GRAPHENE_API void render(const std::shared_ptr<Camera> camera);

private:
    enum BindPoints {
        BIND_MATERIAL = 0,
        BIND_LIGHT = 0
    };

    enum TextureUnits {
        TEXTURE_DIFFUSE,
        TEXTURE_SPECULAR,
        TEXTURE_POSITION,
        TEXTURE_NORMAL,
        TEXTURE_DEPTH
    };

    enum TransformationMask {
        MASK_TRANSLATION = 1 << 0,
        MASK_ROTATION = 1 << 1,
        MASK_SCALING = 1 << 2
    };

    typedef struct {
        Math::Mat4 translation;
        Math::Mat4 rotation;
        Math::Mat4 scaling;
    } Transformation;

    typedef std::function<void(const std::shared_ptr<Object>, const Transformation&)> ObjectHandler;

    void renderShadows(const std::shared_ptr<Camera> camera);
    void renderLights(const std::shared_ptr<Camera> camera);

    Math::Mat4 calculateModelView(const std::shared_ptr<Camera> camera);
    void traverseScene(ObjectHandler handler);

    std::shared_ptr<SceneNode> rootNode;
    std::shared_ptr<Mesh> frame;

    std::shared_ptr<Shader> geometryShader;
    std::shared_ptr<Shader> ambientShader;
    std::shared_ptr<Shader> lightingShader;

    Math::Vec3 ambientColor;
    float ambientEnergy = 1.0f;

    bool shadowPass = false;
    bool lightPass = false;
};

}  // namespace Graphene

#endif  // SCENEMANAGER_H
