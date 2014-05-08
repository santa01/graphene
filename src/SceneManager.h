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

#include <NonCopyable.h>
#include <SceneNode.h>
#include <Camera.h>
#include <Shader.h>
#include <Mesh.h>
#include <Vec3.h>
#include <stdexcept>
#include <algorithm>
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

template<typename Function>
void traverseScene(std::shared_ptr<SceneNode> node, Function function) {
    auto objects = node->getObjects();
    std::for_each(objects.begin(), objects.end(), function);

    auto nodes = node->getNodes();
    std::for_each(nodes.begin(), nodes.end(), [&function](std::shared_ptr<SceneNode> node) {
        traverseScene(node, function);
    });
}

class SceneManager: public std::enable_shared_from_this<SceneManager>, public NonCopyable {
public:
    SceneManager();

    /* const version of shared_from_this() is selected otherwise */
    std::shared_ptr<SceneNode> createNode() {
        return std::make_shared<SceneNode>(this->shared_from_this());
    }

    /* shared_from_this() cannot be called from constructor */
    std::shared_ptr<SceneNode> getRootNode() {
        if (this->rootNode == nullptr) {
            this->rootNode = this->createNode();
        }

        return this->rootNode;
    }

    std::shared_ptr<Shader> getGeometryShader() {
        return this->geometryShader;
    }

    void setGeometryShader(const std::shared_ptr<Shader> geometryShader) {
        if (geometryShader == nullptr) {
            throw std::invalid_argument("Shader cannot be nullptr");
        }

        this->geometryShader = geometryShader;
    }

    std::shared_ptr<Shader> getAmbientShader() {
        return this->ambientShader;
    }

    void setAmbientShader(const std::shared_ptr<Shader> ambientShader) {
        if (ambientShader == nullptr) {
            throw std::invalid_argument("Shader cannot be nullptr");
        }

        this->ambientShader = ambientShader;
    }

    std::shared_ptr<Shader> getLightingShader() {
        return this->lightingShader;
    }

    void setLightingShader(const std::shared_ptr<Shader> lightingShader) {
        if (lightingShader == nullptr) {
            throw std::invalid_argument("Shader cannot be nullptr");
        }

        this->lightingShader = lightingShader;
    }

    const Math::Vec3& getAmbientColor() const {
        return this->ambientColor;
    }

    void setAmbientColor(const Math::Vec3& ambientColor) {
        this->ambientColor = ambientColor;
    }

    float getAmbientEnergy() const {
        return this->ambientEnergy;
    }

    void setAmbientEnergy(float ambientEnergy) {
        if (ambientEnergy < 0.0f) {
            throw std::runtime_error("Ambient energy is less than 0.0f");
        }

        this->ambientEnergy = ambientEnergy;
    }

    bool hasShadowPass() const {
        return this->shadowPass;
    }

    void setShadowPass(bool shadowPass) {
        this->shadowPass = shadowPass;
    }

    bool hasLightPass() const {
        return this->lightPass;
    }

    void setLightPass(bool lightPass) {
        this->lightPass = lightPass;
    }

    void render(const std::shared_ptr<Camera> camera);

private:
    enum BindPoints {
        BIND_MATERIAL = 0,
        BIND_LIGHT    = 0
    };

    enum TextureUnits {
        TEXTURE_DIFFUSE,
        TEXTURE_SPECULAR,
        TEXTURE_POSITION,
        TEXTURE_NORMAL,
        TEXTURE_DEPTH
    };

    void renderShadows();
    void renderLights();

    std::shared_ptr<SceneNode> rootNode;
    std::shared_ptr<Mesh> frame;

    std::shared_ptr<Shader> geometryShader;
    std::shared_ptr<Shader> ambientShader;
    std::shared_ptr<Shader> lightingShader;

    Math::Vec3 ambientColor;
    float ambientEnergy;

    bool shadowPass;
    bool lightPass;
};

}  // namespace Graphene

#endif  // SCENEMANAGER_H
