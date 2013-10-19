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

namespace Graphene {

ImageTexture::ImageTexture(const std::string& name) {
    this->ready = false;

    SDL_Surface* image = IMG_Load(name.c_str());
    if (image == nullptr) {
        return;
    }

    SDL_Surface* source = image;
    if (image->format->BytesPerPixel != 4) {
        SDL_Surface* rgbaImage = SDL_CreateRGBSurface(0, image->w, image->h, 32,
                0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        if (rgbaImage == nullptr) {
            return;
        }

        if (SDL_BlitSurface(image, nullptr, rgbaImage, nullptr) == -1) {
            SDL_FreeSurface(rgbaImage);
            return;
        }

        source = rgbaImage;
    }

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, source->w, source->h, 0,
            (source->format->Rmask > source->format->Bmask) ? GL_BGRA : GL_RGBA,
            GL_UNSIGNED_BYTE, source->pixels);
    this->width = source->w;
    this->height = source->h;

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->ready = true;

    if (source != image) {
        SDL_FreeSurface(source);
    }
}

ImageTexture::ImageTexture(int width, int height) {
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    this->width = width;
    this->height = height;

    glBindTexture(GL_TEXTURE_2D, 0);
    this->ready = true;
}

}  // namespace Graphene
