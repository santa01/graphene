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

#include <TgaImage.h>
#include <Logger.h>
#include <cstdint>
#include <fstream>
#include <stdexcept>

namespace Graphene {

enum class ColorMapType: uint8_t {
    NOT_INCLUDED,
    INCLUDED       // Unsupported
};

enum class ImageType: uint8_t {
    NODATA,                    // Unsupported
    UNCOMPRESSED_COLORMAPPED,  // Unsupported
    UNCOMPRESSED_TRUECOLOR,
    UNCOMPRESSED_BLACKWHITE,   // Unsupported
    RLE_COLORMAPPED = 9,       // Unsupported
    RLE_TRUECOLOR,             // Unsupported
    RLE_BLACKWHITE             // Unsupported
};

enum class ColumnOrdering { LEFT_TO_RIGHT, RIGHT_TO_LEFT };
enum class RowOrdering { BOTTOM_TO_TOP, TOP_TO_BOTTOM };

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

TgaImage::TgaImage(const std::string& filename):
        filename(filename) {
    std::ifstream image(this->filename.c_str(), std::ios::binary);
    if (!image) {
        throw std::runtime_error(LogFormat("std::ifstream()"));
    }

    Header header = { };
    image.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.colorMapType != ColorMapType::NOT_INCLUDED) {
        throw std::runtime_error(LogFormat("Unsupported TGA Colormap Type"));
    }

    if (header.imageType != ImageType::UNCOMPRESSED_TRUECOLOR) {
        throw std::runtime_error(LogFormat("Unsupported TGA Image Type"));
    }

    // attributeBitsPerPixel is bits per alpha channel for 32 and 24 bit images
    if ((header.imageSpec.depth != 32 || header.imageSpec.imageDescr.attributeBitsPerPixel != 8) &&
        (header.imageSpec.depth != 24 || header.imageSpec.imageDescr.attributeBitsPerPixel != 0)) {
        throw std::runtime_error(LogFormat("Unsupported TGA Bit Depth"));
    }

    this->width = header.imageSpec.width;
    this->height = header.imageSpec.height;
    this->pixelDepth = header.imageSpec.depth;

    int pixelBytes = this->pixelDepth >> 3;
    int pixelsRowSize = this->width * pixelBytes;

    this->pixelsSize = this->height * pixelsRowSize;
    this->pixels.reset(new char[this->pixelsSize]);

    /*
     * Per glTexImage2D() documentation: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
     *
     * The first element corresponds to the lower left corner of the texture image. Subsequent elements progress
     * left-to-right through the remaining texels in the lowest row of the texture image, and then in successively
     * higher rows of the texture image. The final element corresponds to the upper right corner of the texture image.
     *
     * Per Truevision TGA FILE FORMAT SPECIFICATION Version 2.0:
     *
     * Image Specification - Field 5 (10 bytes):
     * Bits 5 & 4: These bits are used to indicate the order in which pixel data is transferred from the file to the screen.
     * Bit 4 is for left-to-right ordering and bit 5 is for top-to-bottom ordering as shown below.
     */
    uint8_t rowOrdering = header.imageSpec.imageDescr.topToBottomOrdering;
    uint8_t columnOrdering = header.imageSpec.imageDescr.leftToRightOrdering;
    bool flipRows = (static_cast<RowOrdering>(rowOrdering) == RowOrdering::TOP_TO_BOTTOM);
    bool flipColumns = (static_cast<ColumnOrdering>(columnOrdering) == ColumnOrdering::RIGHT_TO_LEFT);

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

const std::string& TgaImage::getFilename() const {
    return this->filename;
}

}  // namespace Graphene
