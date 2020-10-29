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

#include <ImageTexture.h>
#include <EngineConfig.h>
#include <OpenGL.h>
#include <Logger.h>
#include <algorithm>

namespace Graphene {

ImageTexture::ImageTexture(const Image& image):
        RgbaTexture(image.getWidth(), image.getHeight()) {
    glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int anisotropy = GetEngineConfig().getAnisotropy();
    if (anisotropy > 0 && OpenGL::isExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
        GLint maxAnisotropy;
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
        glTexParameteri(this->target, GL_TEXTURE_MAX_ANISOTROPY, std::min<int>(anisotropy, maxAnisotropy));
    } else {
        LogWarn("GL_ARB_texture_filter_anisotropic unavailable, anisotropic filtering disabled");
    }

    this->update(image);
}

void ImageTexture::update(const Image& image) {
    this->bind();

    GLenum format = (image.getPixelDepth() == 32) ? GL_BGRA : GL_BGR;  // Little-endian ARGB or RGB format
    glTexSubImage2D(this->target, 0, 0, 0, this->width, this->height, format, GL_UNSIGNED_BYTE, image.getPixels());
    glGenerateMipmap(this->target);
}

ImageCubeTexture::ImageCubeTexture(const CubeImage& cubeImage):
        RgbaCubeTexture(cubeImage[0]->getWidth(), cubeImage[0]->getHeight()) {
    glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int anisotropy = GetEngineConfig().getAnisotropy();
    if (anisotropy > 0 && OpenGL::isExtensionSupported("GL_ARB_texture_filter_anisotropic")) {
        GLint maxAnisotropy;
        glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
        glTexParameteri(this->target, GL_TEXTURE_MAX_ANISOTROPY, std::min<int>(anisotropy, maxAnisotropy));
    } else {
        LogWarn("GL_ARB_texture_filter_anisotropic unavailable, anisotropic filtering disabled");
    }

    this->update(cubeImage);
}

void ImageCubeTexture::update(const CubeImage& cubeImage) {
    this->bind();

    for (int faceOffset = 0; faceOffset < 6; faceOffset++) {
        auto faceImage = cubeImage[faceOffset];
        GLenum faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceOffset;
        GLenum faceFormat = (faceImage->getPixelDepth() == 32) ? GL_BGRA : GL_BGR;  // Little-endian ARGB or RGB format
        glTexSubImage2D(faceTarget, 0, 0, 0, faceImage->getWidth(), faceImage->getHeight(), faceFormat, GL_UNSIGNED_BYTE, faceImage->getPixels());
    }

    glGenerateMipmap(this->target);
}

}  // namespace Graphene
