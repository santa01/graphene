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
#include <SDL2/SDL_image.h>
#include <stdexcept>

namespace Graphene {

ImageTexture::ImageTexture(const std::string& name) {
    SDL_Surface* source = IMG_Load(name.c_str());
    if (source == nullptr) {
        throw std::runtime_error("Failed to open `" + name + "'");
    }

    SDL_Surface* image = source;
    if (source->format->BytesPerPixel != 4) {
        image = SDL_CreateRGBSurface(0, source->w, source->h, 32,
                0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        SDL_BlitSurface(source, nullptr, image, nullptr);
    }

    this->width = image->w;
    this->height = image->h;

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexStorage2D(GL_TEXTURE_2D, 8, GL_SRGB8_ALPHA8, this->width, this->height);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height,
            (image->format->Rmask > image->format->Bmask) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (image != source) {
        SDL_FreeSurface(image);
    }
}

}  // namespace Graphene
