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

enum TextureUnit {
    TEXTURE_DIFFUSE,   // GL_TEXTURE0
    TEXTURE_SPECULAR,  // GL_TEXTURE1
    TEXTURE_POSITION,  // GL_TEXTURE2
    TEXTURE_NORMAL,    // GL_TEXTURE3
    TEXTURE_DEPTH      // GL_TEXTURE4
};

class Texture: public NonCopyable {
public:
    GRAPHENE_API Texture(int width, int height, GLenum type, GLenum format, GLsizei mipmaps);
    GRAPHENE_API virtual ~Texture();

    GRAPHENE_API int getWidth() const;
    GRAPHENE_API int getHeight() const;
    GRAPHENE_API GLuint getHandle() const;

    GRAPHENE_API void bind();
    GRAPHENE_API void bind(TextureUnit textureUnit);

protected:
    int width = 0;
    int height = 0;

    GLenum target = 0;
    GLuint texture = 0;

    static TextureUnit activeUnit;
    static GLuint activeTexture;
};

class Texture2D: public Texture {
public:
    GRAPHENE_API Texture2D(int width, int height, GLenum format, GLsizei mipmaps):
            Texture(width, height, GL_TEXTURE_2D, format, mipmaps) {
    }
};

class TextureCubeMap: public Texture {
public:
    GRAPHENE_API TextureCubeMap(int width, int height, GLenum format, GLsizei mipmaps):
            Texture(width, height, GL_TEXTURE_CUBE_MAP, format, mipmaps) {
    }
};

template<GLenum format, GLsizei mipmaps>
class Texture2DTemplate: public Texture2D {
public:
    GRAPHENE_API Texture2DTemplate(int width, int height):
            Texture2D(width, height, format, mipmaps) {
    }
};

template<GLenum format, GLsizei mipmaps>
class TextureCubeMapTemplate: public TextureCubeMap {
public:
    GRAPHENE_API TextureCubeMapTemplate(int width, int height):
            TextureCubeMap(width, height, format, mipmaps) {
    }
};

typedef Texture2DTemplate<GL_RGBA16F, 1>           GeometryTexture;
typedef Texture2DTemplate<GL_DEPTH_COMPONENT16, 1> DepthTexture;
typedef Texture2DTemplate<GL_SRGB8_ALPHA8, 4>      RgbaTexture;
typedef TextureCubeMapTemplate<GL_SRGB8_ALPHA8, 4> RgbaCubeTexture;

}  // namespace Graphene

#endif  // TEXTURE_H
