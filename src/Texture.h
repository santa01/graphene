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

#include <NonCopyable.h>
#include <GL/glew.h>

namespace Graphene {

class Texture: public NonCopyable {
public:
    enum TextureUnit {
        TEXTURE_UNIT_0 = GL_TEXTURE0,
        TEXTURE_UNIT_1 = GL_TEXTURE1,
        TEXTURE_UNIT_2 = GL_TEXTURE2,
        TEXTURE_UNIT_3 = GL_TEXTURE3
    };

    Texture() {
        this->width = 0;
        this->height = 0;

        glGenTextures(1, &this->texture);
    }

    virtual ~Texture() {
        glDeleteTextures(1, &this->texture);
    }

    int getWidth() const {
        return this->width;
    }

    int getHeight() const {
        return this->height;
    }

    void bindTo(TextureUnit unit) {
        glActiveTexture(unit);
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }

    static void clear(TextureUnit unit) {
        glActiveTexture(unit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

protected:
    GLuint texture;

    int width;
    int height;
};

}  // namespace Graphene

#endif  // TEXTURE_H
