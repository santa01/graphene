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

#include <Mesh.h>
#include <Logger.h>
#include <stdexcept>

namespace Graphene {

Mesh::Mesh(const std::shared_ptr<Material> material, const std::shared_ptr<Geometry> geometry):
        material(material),
        geometry(geometry) {
}

std::shared_ptr<Material> Mesh::getMaterial() const {
    return this->material;
}

void Mesh::setMaterial(const std::shared_ptr<Material> material) {
    if (material == nullptr) {
        throw std::invalid_argument(LogFormat("Material cannot be nullptr"));
    }

    this->material = material;
}

std::shared_ptr<Geometry> Mesh::getGeometry() const {
    return this->geometry;
}

void Mesh::setGeometry(const std::shared_ptr<Geometry> geometry) {
    if (geometry == nullptr) {
        throw std::invalid_argument(LogFormat("Geometry cannot be nullptr"));
    }

    this->geometry = geometry;
}

}  // namespace Graphene
