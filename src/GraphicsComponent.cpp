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

#include <GraphicsComponent.h>
#include <Logger.h>
#include <stdexcept>
#include <cassert>

namespace Graphene {

GraphicsComponent::GraphicsComponent():
        Component(ComponentType::GRAPHICS) {
}

const std::vector<std::shared_ptr<Material>>& GraphicsComponent::getMaterials() const {
    return this->materials;
}

const std::vector<std::shared_ptr<Mesh>>& GraphicsComponent::getMeshes() const {
    return this->meshes;
}

void GraphicsComponent::addGraphics(const std::shared_ptr<Material>& material, const std::shared_ptr<Mesh>& mesh) {
    if (material == nullptr) {
        throw std::invalid_argument(LogFormat("Material cannot be nullptr"));
    }

    if (mesh == nullptr) {
        throw std::invalid_argument(LogFormat("Mesh cannot be nullptr"));
    }

    this->materials.emplace_back(material);
    this->meshes.emplace_back(mesh);
}

void GraphicsComponent::update() {
    size_t materialsCount = this->materials.size();
    size_t meshesCount = this->meshes.size();
    assert(materialsCount == meshesCount);

    for (size_t i = 0; i < materialsCount; i++) {
        auto& material = this->materials.at(i);
        material->bind(BIND_MATERIAL);

        auto& texture = material->getDiffuseTexture();
        if (texture != nullptr) {
            texture->bind(TEXTURE_DIFFUSE);
        }

        auto& mesh = this->meshes.at(i);
        mesh->render();
    }
}

}  // namespace Graphene
