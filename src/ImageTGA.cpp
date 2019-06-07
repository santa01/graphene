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

#include <ImageTGA.h>
#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace Graphene {

enum class ColorMapType: uint8_t {
    TGA_COLORMAP_NOT_INCLUDED = 0,
    TGA_COLORMAP_INCLUDED = 1       // Unsupported
};

enum class ImageType: uint8_t {
    TGA_IMAGE_NODATA = 0,                    // Unsupported
    TGA_IMAGE_UNCOMPRESSED_COLORMAPPED = 1,  // Unsupported
    TGA_IMAGE_UNCOMPRESSED_TRUECOLOR = 2,
    TGA_IMAGE_UNCOMPRESSED_BLACKWHITE = 3,   // Unsupported
    TGA_RLE_COLORMAPPED = 9,                 // Unsupported
    TGA_RLE_TRUECOLOR = 10,                  // Unsupported
    TGA_RLE_BLACKWHITE = 11                  // Unsupported
};

enum class ColumnOrdering: uint8_t {
    TGA_COLUMN_LEFT_TO_RIGHT = 0,
    TGA_COLUMN_RIGHT_TO_LEFT = 1
};

enum class RowOrdering: uint8_t {
    TGA_ROW_BOTTOM_TO_TOP = 0,
    TGA_ROW_TOP_TO_BOTTOM = 1
};

#pragma pack(push, 1)

typedef struct {
    uint16_t firstEntryIndex;
    uint16_t colorMapLength;
    uint8_t colorMapEntrySize;
} ColorMapSpec;

typedef struct {
    uint8_t attributeBitsPerPixel: 4;
    uint8_t leftToRightOrdering: 1;
    uint8_t topToBottomOrdering: 1;
    uint8_t compatibility: 2;
} ImageDescr;

typedef struct {
    uint16_t xOrigin;       // Unsupported
    uint16_t yOrigin;       // Unsupported
    uint16_t width;
    uint16_t height;
    uint8_t depth;
    ImageDescr imageDescr;
} ImageSpec;

typedef struct {
    uint8_t idLength;           // Unsupported
    ColorMapType colorMapType;
    ImageType imageType;
    ColorMapSpec colorMapSpec;
    ImageSpec imageSpec;
} Header;

#pragma pack(pop)

ImageTGA::ImageTGA(const std::string& filename):
        filename(filename) {
    std::ifstream image(this->filename.c_str(), std::ios::binary);
    if (!image) {
        throw std::runtime_error("std::ifstream()");
    }

    Header header = { };
    image.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.colorMapType != ColorMapType::TGA_COLORMAP_NOT_INCLUDED) {
        throw std::runtime_error("Unsupported TGA Colormap Type");
    }

    if (header.imageType != ImageType::TGA_IMAGE_UNCOMPRESSED_TRUECOLOR) {
        throw std::runtime_error("Unsupported TGA Image Type");
    }

    // attributeBitsPerPixel is bits per alpha channel for 32 and 24 bit images
    if ((header.imageSpec.depth != 32 || header.imageSpec.imageDescr.attributeBitsPerPixel != 8) &&
        (header.imageSpec.depth != 24 || header.imageSpec.imageDescr.attributeBitsPerPixel != 0)) {
        throw std::runtime_error("Unsupported TGA Bit Depth");
    }

    this->width = header.imageSpec.width;
    this->height = header.imageSpec.height;
    this->pixelDepth = header.imageSpec.depth;

    int pixelBytes = this->pixelDepth >> 3;
    int pixelsSize = this->height * this->width * pixelBytes;
    int pixelsRowSize = this->width * pixelBytes;

    this->pixels.reset(new char[pixelsSize]);

    uint8_t rowOrdering = header.imageSpec.imageDescr.topToBottomOrdering;
    uint8_t columnOrdering = header.imageSpec.imageDescr.leftToRightOrdering;
    bool flipRows = (static_cast<RowOrdering>(rowOrdering) == RowOrdering::TGA_ROW_BOTTOM_TO_TOP);
    bool flipColumns = (static_cast<ColumnOrdering>(columnOrdering) == ColumnOrdering::TGA_COLUMN_RIGHT_TO_LEFT);

    std::ifstream::pos_type pixelsOffset = sizeof(header) + header.idLength +
        (header.colorMapSpec.colorMapLength * header.colorMapSpec.colorMapEntrySize);
    image.seekg(pixelsOffset, std::ios::beg);

    for (int row = 0; row < this->height; row++) {
        int rowOffset = (flipRows ? this->height - row - 1 : row) * pixelsRowSize;

        for (int column = 0; column < this->width; column++) {
            int columnOffset = (flipColumns ? this->width - column - 1 : column) * pixelBytes;

            image.read(this->pixels.get() + rowOffset + columnOffset, pixelBytes);
        }
    }
}

int ImageTGA::getWidth() const {
    return this->width;
}

int ImageTGA::getHeights() const {
    return this->height;
}

int ImageTGA::getPixelDepth() const {
    return this->pixelDepth;
}

const std::string& ImageTGA::getFilename() const {
    return this->filename;
}

const void* ImageTGA::getPixels() const {
    return this->pixels.get();
}

}  // namespace Graphene
