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

#include <RenderComponent.h>
#include <Logger.h>
#include <stdexcept>

namespace Graphene {

RenderComponent::RenderComponent(const std::shared_ptr<Mesh>& mesh):
        Component(ComponentType::RENDER) {
    this->setMesh(mesh);
}

RenderComponent::RenderComponent(const std::shared_ptr<Material>& material, const std::shared_ptr<Mesh>& mesh):
        RenderComponent(mesh) {
    this->setMaterial(material);
}

const std::shared_ptr<Material>& RenderComponent::getMaterial() const {
    return this->material;
}

void RenderComponent::setMaterial(const std::shared_ptr<Material>& material) {
    this->material = material;
}

const std::shared_ptr<Mesh>& RenderComponent::getMesh() const {
    return this->mesh;
}

void RenderComponent::setMesh(const std::shared_ptr<Mesh>& mesh) {
    if (mesh == nullptr) {
        throw std::invalid_argument(LogFormat("Mesh cannot be nullptr"));
    }

    this->mesh = mesh;
}

void RenderComponent::update() {
    if (material != nullptr) {
        material->bind(BIND_MATERIAL);

        auto& texture = material->getDiffuseTexture();
        if (texture != nullptr) {
            texture->bind(TEXTURE_DIFFUSE);
        }
    }

    mesh->render();
}

}  // namespace Graphene
