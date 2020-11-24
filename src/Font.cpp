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

#include <Font.h>
#include <Logger.h>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cassert>
#include <stdexcept>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace Graphene {

void Font::initFreeType() {
    if (Font::library == nullptr) {
        FT_Library library;
        if (FT_Init_FreeType(&library) != FT_Err_Ok) {
            throw std::runtime_error(LogFormat("FT_Init_FreeType()"));
        }

        Font::library.reset(library, FT_Done_FreeType);
    }
}

std::shared_ptr<FT_LibraryRec> Font::library;

Font::Font(const std::string& filename, int size, int dpi):
        filename(filename),
        size(size),
        dpi(dpi) {
    Font::initFreeType();

    FT_Face face;
    if (FT_New_Face(Font::library.get(), this->filename.c_str(), 0, &face) != FT_Err_Ok) {
        throw std::runtime_error(LogFormat("FT_New_Face()"));
    }

    this->face.reset(face, FT_Done_Face);

    FT_F26Dot6 fontSize = this->size << 6;  // Size in 1/64th of a pixel
    if (FT_Set_Char_Size(this->face.get(), fontSize, 0, this->dpi, 0) != FT_Err_Ok) {
        throw std::runtime_error(LogFormat("FT_Set_Char_Size()"));
    }
}

const std::string& Font::getFilename() const {
    return this->filename;
}

int Font::getSize() const {
    return this->size;
}

int Font::getDPI() const {
    return this->dpi;
}

void Font::renderChar(wchar_t charCode, const std::shared_ptr<RawImage>& image) {
    int imagePixelBytes = image->getPixelDepth() >> 3;
    if (imagePixelBytes != 4) {
        throw std::invalid_argument(LogFormat("Image should have 32 bits per pixel"));
    }

    auto& charGlyph = this->getCharGlyph(charCode);
    FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(charGlyph->record.get());
    FT_Bitmap charBitmap = bitmapGlyph->bitmap;

    int imageWidth = image->getWidth();
    if (static_cast<unsigned int>(imageWidth) < charBitmap.width) {
        throw std::runtime_error(LogFormat("Image width is %d but character requires %d", imageWidth, charBitmap.width));
    }

    int imageHeight = image->getHeight();
    if (static_cast<unsigned int>(imageHeight) < charBitmap.rows) {
        throw std::runtime_error(LogFormat("Image height is %d but character requires %d", imageHeight, charBitmap.rows));
    }

    int imagePixelsSize = image->getPixelsSize();
    int imageRowSize = imageWidth * imagePixelBytes;
    int charRowSize = charBitmap.width;  // Single byte (alpha)

    unsigned char* imagePixels = reinterpret_cast<unsigned char*>(image->getPixels());
    const unsigned char* charPixels = charGlyph->pixels.get();

    for (unsigned int charRow = 0; charRow < charBitmap.rows; charRow++) {
        int imageRowOffset = charRow * imageRowSize;
        int charRowOffset = charRow * charRowSize;

        for (unsigned int alphaPixel = 0; alphaPixel < charBitmap.width; alphaPixel++) {
            int alphaOffset = (alphaPixel * imagePixelBytes) + 3;  // Offset to RGB[A] alpha

            assert(imageRowOffset + alphaOffset < imagePixelsSize);
            imagePixels[imageRowOffset + alphaOffset] = charPixels[charRowOffset + alphaPixel];
        }
    }
}

void Font::renderString(const std::wstring& stringText, const std::shared_ptr<RawImage>& image) {
    int imagePixelBytes = image->getPixelDepth() >> 3;
    if (imagePixelBytes != 4) {
        throw std::invalid_argument(LogFormat("RawImage should have 32 bits per pixel"));
    }

    FT_BBox stringBox = { };
    std::vector<std::shared_ptr<CharGlyph>> stringGlyphs;

    for (auto charCode: stringText) {
        auto& charGlyph = this->getCharGlyph(charCode);
        stringBox.yMin = std::min<int>(stringBox.yMin, charGlyph->box->yMin);
        stringBox.yMax = std::max<int>(stringBox.yMax, charGlyph->box->yMax);
        stringBox.xMax += charGlyph->record->advance.x >> 16;  // 16.16 fixed float format

        stringGlyphs.emplace_back(charGlyph);
    }

    int stringWidth = stringBox.xMax + 1;  // Extra pixel in case string box is miscalculated
    int stringHeight = stringBox.yMax - stringBox.yMin;
    int stringAdvance = 0;

    int imageWidth = image->getWidth();
    if (imageWidth < stringWidth) {
        throw std::runtime_error(LogFormat("Image width is %d but string requires %d", imageWidth, stringWidth));
    }

    int imageHeight = image->getHeight();
    if (imageHeight < stringHeight) {
        throw std::runtime_error(LogFormat("Image height is %d but string requires %d", imageHeight, stringHeight));
    }

    int imagePixelsSize = image->getPixelsSize();
    unsigned char* imagePixels = reinterpret_cast<unsigned char*>(image->getPixels());

    for (auto& charGlyph: stringGlyphs) {
        FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(charGlyph->record.get());
        FT_Bitmap charBitmap = bitmapGlyph->bitmap;
        unsigned char* charPixels = charGlyph->pixels.get();

        int stringRow = stringBox.yMax - charGlyph->box->yMax;  // Character Y offset in string
        int charRowSize = charBitmap.width;  // Single byte (alpha)

        for (unsigned int charRow = 0; charRow < charBitmap.rows; charRow++, stringRow++) {
            int imagePixelsOffset = (stringRow * imageWidth + stringAdvance) * imagePixelBytes;
            int charRowOffset = charRow * charRowSize;

            for (unsigned int alphaPixel = 0; alphaPixel < charBitmap.width; alphaPixel++) {
                int alphaOffset = (alphaPixel * imagePixelBytes) + 3;  // Offset to RGB[A] alpha

                assert(imagePixelsOffset + alphaOffset < imagePixelsSize);
                imagePixels[imagePixelsOffset + alphaOffset] = charPixels[charRowOffset + alphaPixel];
            }
        }

        stringAdvance += charGlyph->record->advance.x >> 16;  // 16.16 fixed float format
    }
}

const std::shared_ptr<Font::CharGlyph>& Font::getCharGlyph(wchar_t charCode) {
    auto charGlyphIt = this->charGlyphs.find(charCode);
    if (charGlyphIt != this->charGlyphs.end()) {
        return charGlyphIt->second;
    }

    FT_UInt charIndex = FT_Get_Char_Index(this->face.get(), charCode);
    if (FT_Load_Glyph(this->face.get(), charIndex, FT_LOAD_RENDER) != FT_Err_Ok) {
        throw std::runtime_error(LogFormat("FT_Load_Glyph()"));
    }

    FT_GlyphSlot glyphSlot = this->face->glyph;
    if (glyphSlot->format != FT_GLYPH_FORMAT_BITMAP) {
        throw std::runtime_error(LogFormat("Invalid glyph format"));
    }

    FT_Glyph glyph;
    if (FT_Get_Glyph(glyphSlot, &glyph)) {
        throw std::runtime_error(LogFormat("FT_Get_Glyph()"));
    }

    auto charGlyph = std::make_shared<CharGlyph>();
    charGlyph->record = std::shared_ptr<FT_GlyphRec>(glyph, FT_Done_Glyph);

    charGlyph->box = std::make_shared<FT_BBox>();
    FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_PIXELS, charGlyph->box.get());

    // Invert the control box Y coordinates to reposition inverted bitmap
    int yMin = charGlyph->box->yMin;
    int yMax = charGlyph->box->yMax;
    charGlyph->box->yMin = -yMax;
    charGlyph->box->yMax = -yMin;

    FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
    FT_Bitmap charBitmap = bitmapGlyph->bitmap;

    int bitmapSize = charBitmap.width * charBitmap.rows;
    charGlyph->pixels = std::shared_ptr<unsigned char[]>(new unsigned char[bitmapSize]);
    unsigned char* pixels = charGlyph->pixels.get();

    // Bitmap is inverted vertically relative to the texture coordinates space
    // and contains alpha (transparency) values only (no color information).
    for (unsigned int charRow = 0; charRow < charBitmap.rows; charRow++) {
        int pixelsRowOffset = charRow * charBitmap.width;
        int charRowOffset = (charBitmap.rows - charRow - 1) * charBitmap.width;
        std::memcpy(pixels + pixelsRowOffset, charBitmap.buffer + charRowOffset, charBitmap.width);
    }

    return this->charGlyphs.emplace(charCode, charGlyph).first->second;
}

}  // namespace Graphene
