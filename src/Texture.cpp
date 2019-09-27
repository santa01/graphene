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

#include <Texture.h>

namespace Graphene {

Texture::Texture(int width, int height, GLenum type, GLenum format, GLsizei mipmaps):
        width(width),
        height(height),
        target(type) {
    glGenTextures(1, &this->texture);

    glBindTexture(this->target, this->texture);
    glTexStorage2D(this->target, mipmaps, format, this->width, this->height);
}

Texture::~Texture() {
    glDeleteTextures(1, &this->texture);
}

int Texture::getWidth() const {
    return this->width;
}

int Texture::getHeight() const {
    return this->height;
}

GLuint Texture::getHandle() const {
    return this->texture;
}

void Texture::bind(int textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(this->target, this->texture);
}

}  // namespace Graphene
