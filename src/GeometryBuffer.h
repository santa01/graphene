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

#ifndef GEOMETRYBUFFER_H
#define GEOMETRYBUFFER_H

#include <GrapheneApi.h>
#include <RenderTarget.h>
#include <Texture.h>
#include <memory>

namespace Graphene {

/*
 * Geometry buffer layout
 *
 * Tex0 (RGBA16F): | diffuse R  | diffuse G  | diffuse B  | ambient intensity  |
 * Tex1 (RGBA16F): | specular R | specular G | specular B | diffuse intensity  |
 * Tex2 (RGBA16F): | position X | position Y | position Z | specular intensity |
 * Tex3 (RGBA16F): | normal X   | normal Y   | normal Z   | specular hardness  |
 * Tex4 (DEPTH16): | depth      |
 */

class GeometryBuffer: public RenderTarget {
public:
    GRAPHENE_API GeometryBuffer(int width, int height);
    GRAPHENE_API ~GeometryBuffer();

    GRAPHENE_API const std::shared_ptr<GeometryTexture>& getDiffuseTexture() const;
    GRAPHENE_API const std::shared_ptr<GeometryTexture>& getSpecularTexture() const;
    GRAPHENE_API const std::shared_ptr<GeometryTexture>& getPositionTexture() const;
    GRAPHENE_API const std::shared_ptr<GeometryTexture>& getNormalTexture() const;
    GRAPHENE_API const std::shared_ptr<DepthTexture>& getDepthTexture() const;

    GRAPHENE_API void update() override;

private:
    std::shared_ptr<GeometryTexture> diffuseTexture;
    std::shared_ptr<GeometryTexture> specularTexture;
    std::shared_ptr<GeometryTexture> positionTexture;
    std::shared_ptr<GeometryTexture> normalTexture;
    std::shared_ptr<DepthTexture> depthTexture;
};

}  // namespace Graphene

#endif  // GEOMETRYBUFFER_H
