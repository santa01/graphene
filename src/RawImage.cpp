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

#include <RawImage.h>
#include <Logger.h>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <stdexcept>

namespace Graphene {

RawImage::RawImage(int width, int height, int pixelDepth) {
    this->width = width;
    this->height = height;
    this->pixelDepth = pixelDepth;

    this->pixelsSize = this->height * this->width * (this->pixelDepth >> 3);
    this->pixels.reset(new char[this->pixelsSize]);

    this->clear();
}

RawImage::RawImage(int width, int height, int pixelDepth, const void* pixels):
        RawImage(width, height, pixelDepth) {
    const char* pixelsData = reinterpret_cast<const char*>(pixels);
    std::copy(pixelsData, pixelsData + this->pixelsSize, this->pixels.get());
}

void RawImage::clear(const void* pixel, int pixelDepth) const {
    if (pixelDepth != this->pixelDepth) {
        throw std::runtime_error(LogFormat("Pixel depth is %d but image requires %d", pixelDepth, this->pixelDepth));
    }

    int pixelBytes = pixelDepth >> 3;
    int imageSize = this->height * this->width;
    char* imagePixels = this->pixels.get();

    for (int pixelOffset = 0; pixelOffset < this->pixelsSize; pixelOffset += pixelBytes) {
        assert(pixelOffset + pixelBytes <= this->pixelsSize);
        std::memcpy(imagePixels + pixelOffset, pixel, pixelBytes);
    }
}

void RawImage::clear() const {
    std::memset(this->pixels.get(), 0, this->pixelsSize);
}

void* RawImage::getPixels() const {
    return this->pixels.get();
}

}  // namespace Graphene
