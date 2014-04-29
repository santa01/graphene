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
#include <Sources.h>

namespace Graphene {

SceneManager::SceneManager():
        geometryShader(new Shader(Sources::geometryShader, sizeof(Sources::geometryShader))),
        ambientShader(new Shader(Sources::ambientShader, sizeof(Sources::ambientShader))) {
    this->lightPass = false;
    this->shadowPass = false;

    FrameGeometry geometry = {
        { -1.0f, -1.0f,  0.0f, -1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f,  0.0f },
        {  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f },
        {  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f }, { 0, 1, 3, 1, 2, 3 }
    };
    this->frame = std::make_shared<Mesh>(reinterpret_cast<char*>(&geometry), 2, 4);
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
                auto diffuseTexture = mesh->getMaterial()->getDiffuseTexture();
                if (diffuseTexture != nullptr) {
                    diffuseTexture->bind(0);  // TODO: Do not sample inside shader
                }

                mesh->render();
            }
        }
    });

    RenderStack::pop();  // Geometry textures
    this->ambientShader->enable();
    this->ambientShader->setUniform("diffuseSampler", 0);
    this->ambientShader->setUniform("positionSampler", 1);
    this->ambientShader->setUniform("normalSampler", 2);
    this->ambientShader->setUniform("depthSampler", 3);

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
