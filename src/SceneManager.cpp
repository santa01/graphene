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

#include <SceneManager.h>
#include <RenderStack.h>
#include <Object.h>
#include <Entity.h>
#include <cstring>

namespace Graphene {

const float SceneManager::vertexData[] = {
    -1.0f, -1.0f,  0.0f,
    -1.0f,  1.0f,  0.0f,
     1.0f,  1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,  // Positions
     0.0f,  0.0f,  0.0f,
     0.0f,  0.0f,  0.0f,
     0.0f,  0.0f,  0.0f,
     0.0f,  0.0f,  0.0f,  // Normals
     0.0f,  0.0f,
     0.0f,  1.0f,
     1.0f,  1.0f,
     1.0f,  0.0f          // UVs
};

const int SceneManager::faceData[] = { 0, 1, 3, 1, 2, 3 };

const char SceneManager::geometrySource[] = R"(
    #version 330

    #ifdef TYPE_VERTEX
        uniform mat4 modelViewProjection;
        uniform mat4 localWorld;

        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec3 vertexNormal;
        layout(location = 2) in vec2 vertexUV;

        smooth out vec3 fragmentPosition;
        smooth out vec3 fragmentNormal;
        smooth out vec2 fragmentUV;

        void main () {
            vec4 vertexWorldPosition = localWorld * vec4(vertexPosition, 1.0f);
            vec4 vertexWorldNormal = localWorld * vec4(vertexNormal, 1.0f);
            gl_Position = modelViewProjection * vertexWorldPosition;

            fragmentPosition = vec3(vertexWorldPosition);
            fragmentNormal = vec3(vertexWorldNormal);
            fragmentUV = vertexUV;
        }
    #endif

    #ifdef TYPE_FRAGMENT
        uniform sampler2D diffuseSampler;

        smooth in vec3 fragmentPosition;
        smooth in vec3 fragmentNormal;
        smooth in vec2 fragmentUV;

        layout(location = 0) out vec4 outputDiffuse;
        layout(location = 1) out vec4 outputPosition;
        layout(location = 2) out vec4 outputNormal;

        void main() {
            outputDiffuse = texture(diffuseSampler, fragmentUV);
            outputPosition = vec4(fragmentPosition, 0.0f);
            outputNormal = vec4(fragmentNormal, 0.0f);
        }
    #endif
)";

const char SceneManager::frameSource[] = R"(
    #version 330

    #ifdef TYPE_VERTEX
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec3 vertexNormal;
        layout(location = 2) in vec2 vertexUV;

        smooth out vec2 fragmentUV;

        void main () {
            gl_Position = vec4(vertexPosition, 1.0f);
            fragmentUV = vertexUV;
        }
    #endif

    #ifdef TYPE_FRAGMENT
        uniform sampler2D diffuseSampler;
        uniform sampler2D positionSampler;
        uniform sampler2D normalSampler;
        uniform sampler1D depthSampler;

        smooth in vec2 fragmentUV;

        out vec4 finalColor;

        void main() {
            finalColor = texture(diffuseSampler, fragmentUV);
            //finalColor = texture(positionSampler, fragmentUV);
            //finalColor = texture(normalSampler, fragmentUV);
            //finalColor = texture(depthSampler, fragmentUV);
        }
    #endif
)";

SceneManager::SceneManager():
        geometryShader(new Shader(SceneManager::geometrySource, sizeof(SceneManager::geometrySource))),
        frameShader(new Shader(SceneManager::frameSource, sizeof(SceneManager::frameSource))) {
    this->lightPass = false;
    this->shadowPass = false;

    int geometrySize = sizeof(SceneManager::vertexData) + sizeof(SceneManager::faceData);
    std::unique_ptr<char[]> frameGeometry(new char[geometrySize]);

    char* vertexData = frameGeometry.get();
    char* faceData = vertexData + sizeof(SceneManager::vertexData);

    memcpy(vertexData, SceneManager::vertexData, sizeof(SceneManager::vertexData));
    memcpy(faceData, SceneManager::faceData, sizeof(SceneManager::faceData));
    this->frame = std::make_shared<Mesh>(frameGeometry.get(), 2, 4);
}

void SceneManager::render(const std::shared_ptr<Camera> camera) {
    if (camera == nullptr) {
        throw std::invalid_argument("Camera cannot be nullptr");
    }

    if (RenderStack::depth() < 3) {
        throw std::runtime_error("RenderStack is not populated");
    }

    RenderStack::pop();  // Geometry buffer
    this->geometryShader->enable();
    this->geometryShader->setUniform("diffuseSampler", 0);
    this->geometryShader->setUniform("modelViewProjection",
            camera->getProjection() * camera->getRotation() * camera->getTranslation());

    traverseScene(this->rootNode, [this](std::shared_ptr<Object> object) {
        if (object->getObjectType() == Object::ObjectType::TYPE_ENTITY) {
            auto entity = std::dynamic_pointer_cast<Entity>(object);

            this->geometryShader->setUniform("localWorld",
                    entity->getScaling() * entity->getRotation() * entity->getTranslation());
            for (auto& mesh: entity->getMeshes()) {
                mesh->getMaterial()->getDiffuseTexture()->bind(0);
                mesh->render();
            }
        }
    });

    RenderStack::pop();  // Geometry textures
    this->frameShader->enable();
    this->frameShader->setUniform("diffuseSampler", 0);
    this->frameShader->setUniform("positionSampler", 1);
    this->frameShader->setUniform("normalSampler", 2);
    this->frameShader->setUniform("depthSampler", 3);

    RenderStack::pop();  // Output buffer
    this->frame->render();

    if (this->shadowPass) {
        renderShadows();
    }

    if (this->lightPass) {
        renderLights();
    }
}

void SceneManager::renderShadows() {
    // TODO
}

void SceneManager::renderLights() {
    // TODO
}

}  // namespace Graphene
