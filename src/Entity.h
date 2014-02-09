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

#include <Scalable.h>
#include <Material.h>
#include <Mesh.h>
#include <Object.h>
#include <stdexcept>
#include <memory>

namespace Graphene {

class Entity: public Object, public Scalable {
public:
    Entity(const std::shared_ptr<Material> material, const std::shared_ptr<Mesh> mesh) {
        this->setMaterial(material);
        this->setMesh(mesh);
    }

    /* Rotatable */

    void roll(float angle) {}
    void yaw(float angle) {}
    void pitch(float angle) {}

    float getXAngle() const {}
    float getYAngle() const {}
    float getZAngle() const {}

    void rotate(const Math::Vec3& vector, float angle) {}

    /* Scalable */

    void scaleX(float factor) {}
    void scaleY(float factor) {}
    void scaleZ(float factor) {}

    float getXFactor() const {}
    float getYFactor() const {}
    float getZFactor() const {}

    void scale(float factor) {}

    /* Movable */

    void translate(float x, float y, float z) {}
    void translate(const Math::Vec3& position) {}

    void move(float x, float y, float z) {}
    void move(const Math::Vec3& position) {}

    Math::Vec3 getPosition() const {}

    /* Entity */

    std::shared_ptr<Material> getMaterial() {
        return this->material;
    }

    void setMaterial(const std::shared_ptr<Material> material) {
        if (material == nullptr) {
            throw std::invalid_argument("Material cannot be nullptr");
        }

        this->material = material;
    }

    std::shared_ptr<Mesh> getMesh() {
        return this->mesh;
    }

    void setMesh(const std::shared_ptr<Mesh> mesh) {
        if (mesh == nullptr) {
            throw std::invalid_argument("Mesh cannot be nullptr");
        }

        this->mesh = mesh;
    }

private:
    std::shared_ptr<Material> material;
    std::shared_ptr<Mesh> mesh;
};

}  // namespace Graphene

#endif  // ENTITY_H
