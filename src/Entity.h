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
#include <Quaternion.h>
#include <Vec3.h>
#include <stdexcept>
#include <memory>

namespace Graphene {

class Entity: public Object, public Scalable {
public:
    Entity(const std::shared_ptr<Material> material, const std::shared_ptr<Mesh> mesh) {
        this->objectType = ObjectType::TYPE_ENTITY;
        this->setMaterial(material);
        this->setMesh(mesh);
    }

    /* Rotatable */

    void roll(float angle) {
        this->rotate(Math::Vec3::UNIT_X, angle);
    }

    void yaw(float angle) {
        this->rotate(Math::Vec3::UNIT_Y, angle);
    }

    void pitch(float angle) {
        this->rotate(Math::Vec3::UNIT_Z, angle);
    }

    void rotate(const Math::Vec3& vector, float angle) {
        Math::Vec3 axis(vector);
        Math::Quaternion q(axis.normalize(), angle * M_PI / 180.0f);
        q.normalize();

        float xAngle, yAngle, zAngle;
        q.extractEulerAngles(xAngle, yAngle, zAngle);

        this->rotationAngles += Math::Vec3(xAngle * 180.f / M_PI, yAngle * 180.f / M_PI, zAngle * 180.f / M_PI);
        this->rotation = this->rotation * q.extractMat4();
    }

    Math::Vec3 getRotationAngles() const {
        return this->rotationAngles;
    }

    /* Scalable */

    void scaleX(float factor) {
        this->scale(factor, 1.0f, 1.0f);
    }

    void scaleY(float factor) {
        this->scale(1.0f, factor, 1.0f);
    }

    void scaleZ(float factor) {
        this->scale(1.0f, 1.0f, factor);
    }

    void scale(float xFactor, float yFactor, float zFactor) {
        this->scale(Math::Vec3(xFactor, yFactor, zFactor));
    }

    void scale(const Math::Vec3& factors) {
        this->scaling.set(0, 0, this->scaling.get(0, 0) * factors.get(Math::Vec3::X));
        this->scaling.set(1, 1, this->scaling.get(1, 1) * factors.get(Math::Vec3::Y));
        this->scaling.set(2, 2, this->scaling.get(2, 2) * factors.get(Math::Vec3::Z));
    }

    Math::Vec3 getScalingFactors() const {
        return Math::Vec3(this->scaling.get(0, 0), this->scaling.get(1, 1), this->scaling.get(2, 2));
    }

    /* Movable */

    void translate(float x, float y, float z) {
        this->translate(Math::Vec3(x, y, z));
    }

    void translate(const Math::Vec3& position) {
        this->move(position - this->getPosition());
    }

    void move(float x, float y, float z) {
        this->move(Math::Vec3(x, y, z));
    }

    void move(const Math::Vec3& position) {
        this->translation.set(0, 3, this->translation.get(0, 3) + position.get(Math::Vec3::X));
        this->translation.set(1, 3, this->translation.get(1, 3) + position.get(Math::Vec3::Y));
        this->translation.set(2, 3, this->translation.get(2, 3) + position.get(Math::Vec3::Z));
    }

    Math::Vec3 getPosition() const {
        return Math::Vec3(this->translation.get(0, 3), this->translation.get(1, 3), this->translation.get(2, 3));
    }

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

    Math::Mat4 translation;
    Math::Mat4 rotation;
    Math::Mat4 scaling;

    Math::Vec3 rotationAngles;
};

}  // namespace Graphene

#endif  // ENTITY_H
