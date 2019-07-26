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

#include <GrapheneApi.h>
#include <ImageTexture.h>
#include <UniformBuffer.h>
#include <OpenGL.h>
#include <Vec3.h>
#include <memory>

namespace Graphene {

class Material {
public:
    GRAPHENE_API Material();
    GRAPHENE_API ~Material();

    GRAPHENE_API std::shared_ptr<ImageTexture> getDiffuseTexture() const;
    GRAPHENE_API void setDiffuseTexture(const std::shared_ptr<ImageTexture> diffuseTexture);

    GRAPHENE_API float getAmbientIntensity() const;
    GRAPHENE_API void setAmbientIntensity(float ambientIntensity);

    GRAPHENE_API float getDiffuseIntensity() const;
    GRAPHENE_API void setDiffuseIntensity(float diffuseIntensity);

    GRAPHENE_API float getSpecularIntensity() const;
    GRAPHENE_API void setSpecularIntensity(float specularIntensity);

    GRAPHENE_API int getSpecularHardness() const;
    GRAPHENE_API void setSpecularHardness(int specularHardness);

    GRAPHENE_API const Math::Vec3& getDiffuseColor() const;
    GRAPHENE_API void setDiffuseColor(const Math::Vec3& diffuseColor);

    GRAPHENE_API const Math::Vec3& getSpecularColor() const;
    GRAPHENE_API void setSpecularColor(const Math::Vec3& specularColor);

    GRAPHENE_API std::shared_ptr<UniformBuffer> getMaterialBuffer() const;

    GRAPHENE_API void bind(int bindPoint);

private:
    std::shared_ptr<ImageTexture> diffuseTexture;
    std::shared_ptr<UniformBuffer> materialBuffer;

    float ambientIntensity = 1.0f;
    float diffuseIntensity = 1.0f;
    float specularIntensity = 0.5f;
    int specularHardness = 50;

    Math::Vec3 diffuseColor = { 1.0f, 1.0f, 1.0f };
    Math::Vec3 specularColor = { 1.0f, 1.0f, 1.0f };

    GLuint ubo = 0;
};

}  // namespace Graphene

#endif  // MATERIAL_H
