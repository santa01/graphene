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
#include <RawImage.h>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>

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

std::shared_ptr<Font::FT_LibraryRec> Font::library;

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

std::shared_ptr<Image> Font::renderChar(wchar_t charCode) {
    std::shared_ptr<CharGlyph> charGlyph = this->getCharGlyph(charCode);
    if (charGlyph == nullptr) {
        return nullptr;
    }

    FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(charGlyph->record.get());
    FT_Bitmap charBitmap = bitmapGlyph->bitmap;

    return std::make_shared<RawImage>(charBitmap.width, charBitmap.rows, 32, charGlyph->pixels.get());
}

std::shared_ptr<Image> Font::renderString(const std::wstring& stringText) {
    FT_BBox stringBox = { };
    std::vector<std::shared_ptr<CharGlyph>> stringGlyphs;

    for (auto charCode: stringText) {
        std::shared_ptr<CharGlyph> charGlyph = this->getCharGlyph(charCode);
        if (charGlyph == nullptr) {
            continue;
        }

        stringBox.yMin = std::min<int>(stringBox.yMin, charGlyph->box.yMin);
        stringBox.yMax = std::max<int>(stringBox.yMax, charGlyph->box.yMax);
        stringBox.xMax += charGlyph->record->advance.x >> 16;  // 16.16 fixed float format

        stringGlyphs.push_back(charGlyph);
    }

    auto stringImage = std::make_shared<RawImage>(stringBox.xMax, stringBox.yMax - stringBox.yMin, 32);
    int stringAdvance = 0;

    int pixelBytes = stringImage->getPixelDepth() >> 3;
    char* pixels = stringImage->getRawData();

    for (auto charGlyph: stringGlyphs) {
        FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(charGlyph->record.get());
        FT_Bitmap charBitmap = bitmapGlyph->bitmap;
        char* charPixels = charGlyph->pixels.get();

        int stringRow = stringBox.yMax - charGlyph->box.yMax;  // Character Y offset in string
        int charBitmapRowSize = charBitmap.width * pixelBytes;

        for (unsigned int charRow = 0; charRow < charBitmap.rows; charRow++, stringRow++) {
            int pixelsOffset = (stringRow * stringImage->getWidth() + stringAdvance) * pixelBytes;
            int charPixelsOffset = charRow * charBitmapRowSize;
            memcpy(pixels + pixelsOffset, charPixels + charPixelsOffset, charBitmapRowSize);
        }

        stringAdvance += charGlyph->record->advance.x >> 16;  // 16.16 fixed float format
    }

    return stringImage;
}

std::shared_ptr<Font::CharGlyph> Font::getCharGlyph(FT_ULong charCode) {
    auto charGlyphIt = this->charGlyphs.find(charCode);
    if (charGlyphIt != this->charGlyphs.end()) {
        return charGlyphIt->second;
    }

    FT_UInt charIndex = FT_Get_Char_Index(this->face.get(), charCode);
    if (FT_Load_Glyph(this->face.get(), charIndex, FT_LOAD_RENDER) != FT_Err_Ok) {
        return nullptr;
    }

    FT_GlyphSlot glyphSlot = this->face->glyph;
    if (glyphSlot->format != FT_GLYPH_FORMAT_BITMAP) {
        return nullptr;
    }

    FT_Glyph glyph;
    if (FT_Get_Glyph(glyphSlot, &glyph)) {
        return nullptr;
    }

    std::shared_ptr<CharGlyph> charGlyph(new CharGlyph());
    this->charGlyphs.emplace(charCode, charGlyph);

    charGlyph->record = std::shared_ptr<FT_GlyphRec>(glyph, FT_Done_Glyph);
    FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_PIXELS, &charGlyph->box);

    FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
    FT_Bitmap charBitmap = bitmapGlyph->bitmap;

    int pixelBytes = 4;  // RGBA
    int pixelsSize = charBitmap.width * charBitmap.rows * pixelBytes;
    int bitmapSize = charBitmap.width * charBitmap.rows;

    charGlyph->pixels = std::shared_ptr<char[]>(new char[pixelsSize]);
    char* pixels = charGlyph->pixels.get();

    for (int pixel = 0; pixel < bitmapSize; pixel++) {
        unsigned char pixelData[] = { 0, 0, 0, charBitmap.buffer[pixel] };
        int pixelOffset = pixel * sizeof(pixelData);
        memcpy(pixels + pixelOffset, pixelData, sizeof(pixelData));
    }

    return charGlyph;
}

}  // namespace Graphene
