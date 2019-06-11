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
#include <NonCopyable.h>
#include <ImageTexture.h>
#include <DepthTexture.h>
#include <OpenGL.h>
#include <memory>

namespace Graphene {

/*
 * Geometry buffer layout
 *
 * Tex0 (RGBA16F): | diffuse R  | diffuse G  | diffuse B  | ambient intensity  |
 * Tex1 (RGBA16F): | specular R | specular G | specular B | diffuse intensity  |
 * Tex2 (RGBA16F): | position X | position Y | position Z | specular intensity |
 * Tex3 (RGBA16F): | normal X   | normal Y   | normal Z   | specular hardness  |
 * Tex4 (D16):     | depth      |
 */

class GeometryBuffer: public NonCopyable {
public:
    GRAPHENE_API GeometryBuffer(int width, int height);
    GRAPHENE_API ~GeometryBuffer();

    GRAPHENE_API int getWidth() const;
    GRAPHENE_API int getHeight() const;

    GRAPHENE_API std::shared_ptr<ImageTexture> getDiffuseTexture();
    GRAPHENE_API std::shared_ptr<ImageTexture> getSpecularTexture();
    GRAPHENE_API std::shared_ptr<ImageTexture> getPositionTexture();
    GRAPHENE_API std::shared_ptr<ImageTexture> getNormalTexture();
    GRAPHENE_API std::shared_ptr<DepthTexture> getDepthTexture();

    GRAPHENE_API void bind();

private:
    std::shared_ptr<ImageTexture> diffuseTexture;
    std::shared_ptr<ImageTexture> specularTexture;
    std::shared_ptr<ImageTexture> positionTexture;
    std::shared_ptr<ImageTexture> normalTexture;
    std::shared_ptr<DepthTexture> depthTexture;

    GLenum buffers[4];
    GLuint fbo;

    int width;
    int height;
};

}  // namespace Graphene

#endif  // GEOMETRYBUFFER_H
