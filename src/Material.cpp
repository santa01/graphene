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

#include <Material.h>
#include <Logger.h>
#include <stdexcept>
#include <algorithm>

namespace Graphene {

#pragma pack(push, 1)

/* std140 layout */
typedef struct {
    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    int specularHardness;
    float diffuseColor[3];
    int diffuseTexture;
    float specularColor[3];
} MaterialBuffer;

#pragma pack(pop)

Material::Material() {
    MaterialBuffer material = {
        this->ambientIntensity,
        this->diffuseIntensity,
        this->specularIntensity,
        this->specularHardness,
        { 0.0f, 0.0f, 0.0f }, 0,
        { 0.0f, 0.0f, 0.0f }
    };
    std::copy(this->diffuseColor.data(), this->diffuseColor.data() + 3, material.diffuseColor);
    std::copy(this->specularColor.data(), this->specularColor.data() + 3, material.specularColor);

    this->materialBuffer = std::make_shared<UniformBuffer>(&material, sizeof(material));
}

std::shared_ptr<Texture> Material::getDiffuseTexture() const {
    return this->diffuseTexture;
}

void Material::setDiffuseTexture(const std::shared_ptr<Texture> diffuseTexture) {
    this->diffuseTexture = diffuseTexture;

    bool diffuseTextureAvailable = (this->diffuseTexture != nullptr);
    updateBuffer(this->materialBuffer, MaterialBuffer, diffuseTexture, &diffuseTextureAvailable);
}

float Material::getAmbientIntensity() const {
    return this->ambientIntensity;
}

void Material::setAmbientIntensity(float ambientIntensity) {
    if (ambientIntensity < 0.0f || ambientIntensity > 1.0f) {
        throw std::runtime_error(LogFormat("Ambient intensity is not in [0.0f, 1.0f] range"));
    }

    this->ambientIntensity = ambientIntensity;
    updateBuffer(this->materialBuffer, MaterialBuffer, ambientIntensity, &this->ambientIntensity);
}

float Material::getDiffuseIntensity() const {
    return this->diffuseIntensity;
}

void Material::setDiffuseIntensity(float diffuseIntensity) {
    if (diffuseIntensity < 0.0f || diffuseIntensity > 1.0f) {
        throw std::runtime_error(LogFormat("Diffuse intensity is not in [0.0f, 1.0f] range"));
    }

    this->diffuseIntensity = diffuseIntensity;
    updateBuffer(this->materialBuffer, MaterialBuffer, diffuseIntensity, &this->diffuseIntensity);
}

float Material::getSpecularIntensity() const {
    return this->specularIntensity;
}

void Material::setSpecularIntensity(float specularIntensity) {
    if (specularIntensity < 0.0f || specularIntensity > 1.0f) {
        throw std::runtime_error(LogFormat("Specular intensity is not in [0.0f, 1.0f] range"));
    }

    this->specularIntensity = specularIntensity;
    updateBuffer(this->materialBuffer, MaterialBuffer, specularIntensity, &this->specularIntensity);
}

int Material::getSpecularHardness() const {
    return this->specularHardness;
}

void Material::setSpecularHardness(int specularHardness) {
    if (specularHardness < 0) {
        throw std::runtime_error(LogFormat("Specular hardness is less than 0"));
    }

    this->specularHardness = specularHardness;
    updateBuffer(this->materialBuffer, MaterialBuffer, specularHardness, &this->specularHardness);
}

const Math::Vec3& Material::getDiffuseColor() const {
    return this->diffuseColor;
}

void Material::setDiffuseColor(const Math::Vec3& diffuseColor) {
    this->diffuseColor = diffuseColor;
    updateBuffer(this->materialBuffer, MaterialBuffer, diffuseColor, this->diffuseColor.data());
}

const Math::Vec3& Material::getSpecularColor() const {
    return this->specularColor;
}

void Material::setSpecularColor(const Math::Vec3& specularColor) {
    this->specularColor = specularColor;
    updateBuffer(this->materialBuffer, MaterialBuffer, specularColor, this->specularColor.data());
}

std::shared_ptr<UniformBuffer> Material::getMaterialBuffer() const {
    return this->materialBuffer;
}

}  // namespace Graphene
