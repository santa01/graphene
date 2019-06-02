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
#include <ImageTGA.h>
#include <OpenGL.h>
#include <stdexcept>

namespace Graphene {

ImageTexture::ImageTexture(const std::string& name) {
    ImageTGA image(name);
    this->width = image.getWidth();
    this->height = image.getHeights();

    // Little-endian ARGB or RGB format
    GLenum format = (image.getPixelDepth() == 32) ? GL_BGRA : GL_BGR;

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexStorage2D(GL_TEXTURE_2D, 8, GL_SRGB8_ALPHA8, this->width, this->height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, format, GL_UNSIGNED_BYTE, image.getPixels());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

ImageTexture::ImageTexture(int width, int height) {
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, this->width, this->height);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}  // namespace Graphene
