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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <OpenGL.h>

namespace Graphene {

class Texture: public NonCopyable {
public:
    GRAPHENE_API Texture(int width, int height, GLsizei mipmapLevels, GLenum storageFormat);
    GRAPHENE_API virtual ~Texture();

    GRAPHENE_API int getWidth() const;
    GRAPHENE_API int getHeight() const;
    GRAPHENE_API GLuint getHandle() const;

    GRAPHENE_API void bind(int textureUnit);

protected:
    int width = 0;
    int height = 0;
    GLuint texture = 0;
};

template<GLsizei mipmapLevels, GLenum storageFormat>
class TextureTemplate: public Texture {
public:
    GRAPHENE_API TextureTemplate(int width, int height):
            Texture(width, height, mipmapLevels, storageFormat) {
    }
};

typedef TextureTemplate<4, GL_SRGB8_ALPHA8> RgbaTexture;
typedef TextureTemplate<1, GL_DEPTH_COMPONENT16> DepthTexture;
typedef TextureTemplate<1, GL_RGBA16F> GeometryTexture;

}  // namespace Graphene

#endif  // TEXTURE_H
