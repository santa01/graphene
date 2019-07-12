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

#include <Scene.h>
#include <Logger.h>
#include <Object.h>
#include <Mat4.h>
#include <algorithm>
#include <stdexcept>

namespace Graphene {

Scene::Scene():
        ambientColor(1.0f, 1.0f, 1.0f) {
}

std::shared_ptr<SceneNode> Scene::createNode() {
    /* const version of shared_from_this() is selected otherwise */
    return std::make_shared<SceneNode>(this->shared_from_this());
}

std::shared_ptr<SceneNode> Scene::getRootNode() {
    /* shared_from_this() cannot be called from constructor */
    if (this->rootNode == nullptr) {
        this->rootNode = this->createNode();
    }

    return this->rootNode;
}

const Math::Vec3& Scene::getAmbientColor() const {
    return this->ambientColor;
}

void Scene::setAmbientColor(const Math::Vec3& ambientColor) {
    this->ambientColor = ambientColor;
}

float Scene::getAmbientEnergy() const {
    return this->ambientEnergy;
}

void Scene::setAmbientEnergy(float ambientEnergy) {
    if (ambientEnergy < 0.0f) {
        throw std::runtime_error(LogFormat("Ambient energy is less than 0.0f"));
    }

    this->ambientEnergy = ambientEnergy;
}

Math::Mat4 Scene::calculateModelView(const std::shared_ptr<Camera> camera) {
    std::shared_ptr<SceneNode> node = camera->getParent();
    Math::Mat4 modelView;

    while (node != nullptr) {
        // Moving to the root node, current node's transformation matrix is the left operand
        // to be the last operation. Eventually root node's transformation will be the first one.
        modelView = node->getOppositeRotation() * node->getOppositeTranslation() * modelView;
        node = node->getParent();
    }

    modelView = camera->getOppositeRotation() * camera->getOppositeTranslation() * modelView;
    return modelView;
}

void Scene::walkThrough(ObjectHandler handler) {
    std::function<void(const std::shared_ptr<SceneNode>, Math::Mat4)> traverser;
    traverser = [&handler, &traverser](const std::shared_ptr<SceneNode> node, Math::Mat4 transformation) {
        // Moving from the root node, current node's transformation matrix is the left operand
        // to be the last operation. Keep the root node's transformation the last one.
        transformation = node->getTranslation() * node->getRotation() * node->getScaling() * transformation;

        auto objects = node->getObjects();
        std::for_each(objects.begin(), objects.end(), [&handler, &transformation](const std::shared_ptr<Object> object) {
            handler(object, transformation);
        });

        auto nodes = node->getNodes();
        std::for_each(nodes.begin(), nodes.end(), [&traverser, &transformation](const std::shared_ptr<SceneNode> node) {
            traverser(node, transformation);
        });
    };

    Math::Mat4 transformation;
    traverser(this->rootNode, transformation);
}

}  // namespace Graphene
