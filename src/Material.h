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

#ifndef MATERIAL_H
#define MATERIAL_H

#include <ImageTexture.h>
#include <UniformBuffer.h>
#include <OpenGL.h>
#include <Vec3.h>
#include <stdexcept>
#include <memory>
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

class Material {
public:
    Material();

    ~Material() {
        glDeleteBuffers(1, &this->ubo);
    }

    std::shared_ptr<ImageTexture> getDiffuseTexture() {
        return this->diffuseTexture;
    }

    void setDiffuseTexture(const std::shared_ptr<ImageTexture> diffuseTexture) {
        if (diffuseTexture == nullptr) {
            throw std::invalid_argument("ImageTexture cannot be nullptr");
        }

        this->diffuseTexture = diffuseTexture;
        bool diffuseTextureAvailable = true;
        updateBuffer(this->materialBuffer, MaterialBuffer, diffuseTexture, &diffuseTextureAvailable);
    }

    float getAmbientIntensity() const {
        return this->ambientIntensity;
    }

    void setAmbientIntensity(float ambientIntensity) {
        if (ambientIntensity < 0.0f || ambientIntensity > 1.0f) {
            throw std::runtime_error("Ambient intensity is not in [0.0f, 1.0f] range");
        }

        this->ambientIntensity = ambientIntensity;
        updateBuffer(this->materialBuffer, MaterialBuffer, ambientIntensity, &this->ambientIntensity);
    }

    float getDiffuseIntensity() const {
        return this->diffuseIntensity;
    }

    void setDiffuseIntensity(float diffuseIntensity) {
        if (diffuseIntensity < 0.0f || diffuseIntensity > 1.0f) {
            throw std::runtime_error("Diffuse intensity is not in [0.0f, 1.0f] range");
        }

        this->diffuseIntensity = diffuseIntensity;
        updateBuffer(this->materialBuffer, MaterialBuffer, diffuseIntensity, &this->diffuseIntensity);
    }

    float getSpecularIntensity() const {
        return this->specularIntensity;
    }

    void setSpecularIntensity(float specularIntensity) {
        if (specularIntensity < 0.0f || specularIntensity > 1.0f) {
            throw std::runtime_error("Specular intensity is not in [0.0f, 1.0f] range");
        }

        this->specularIntensity = specularIntensity;
        updateBuffer(this->materialBuffer, MaterialBuffer, specularIntensity, &this->specularIntensity);
    }

    int getSpecularHardness() const {
        return this->specularHardness;
    }

    void setSpecularHardness(int specularHardness) {
        if (specularHardness < 0) {
            throw std::runtime_error("Specular hardness is less than 0");
        }

        this->specularHardness = specularHardness;
        updateBuffer(this->materialBuffer, MaterialBuffer, specularHardness, &this->specularHardness);
    }

    const Math::Vec3& getDiffuseColor() const {
        return this->diffuseColor;
    }

    void setDiffuseColor(const Math::Vec3& diffuseColor) {
        this->diffuseColor = diffuseColor;
        updateBuffer(this->materialBuffer, MaterialBuffer, diffuseColor, this->diffuseColor.data());
    }

    const Math::Vec3& getSpecularColor() const {
        return this->specularColor;
    }

    void setSpecularColor(const Math::Vec3& specularColor) {
        this->specularColor = specularColor;
        updateBuffer(this->materialBuffer, MaterialBuffer, specularColor, this->specularColor.data());
    }

    std::shared_ptr<UniformBuffer> getMaterialBuffer() {
        return this->materialBuffer;
    }

    void bind(int bindPoint) {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, this->ubo);
    }

private:
    std::shared_ptr<ImageTexture> diffuseTexture;
    std::shared_ptr<UniformBuffer> materialBuffer;

    float ambientIntensity;
    float diffuseIntensity;
    float specularIntensity;
    int specularHardness;

    Math::Vec3 diffuseColor;
    Math::Vec3 specularColor;

    GLuint ubo;
};

}  // namespace Graphene

#endif  // MATERIAL_H
