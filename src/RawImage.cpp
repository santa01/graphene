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
#include <algorithm>
#include <cstring>

namespace Graphene {

RawImage::RawImage(int width, int height, int pixelDepth) {
    this->width = width;
    this->height = height;
    this->pixelDepth = pixelDepth;

    this->pixelsSize = this->height * this->width * (this->pixelDepth >> 3);
    this->pixels.reset(new char[this->pixelsSize]);
    memset(this->pixels.get(), 0, this->pixelsSize);
}

RawImage::RawImage(int width, int height, int pixelDepth, const void* pixels):
        RawImage(width, height, pixelDepth) {
    const char* pixelsData = reinterpret_cast<const char*>(pixels);
    std::copy(pixelsData, pixelsData + this->pixelsSize, this->pixels.get());
}

char* RawImage::getRawData() {
    return this->pixels.get();
}

}  // namespace Graphene
