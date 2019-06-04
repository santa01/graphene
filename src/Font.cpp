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
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>

namespace Graphene {

void Font::initFreeType() {
    if (Font::library == nullptr) {
        FT_Library library;
        if (FT_Init_FreeType(&library) != FT_Err_Ok) {
            throw std::runtime_error("FT_Init_FreeType()");
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
        throw std::runtime_error("FT_New_Face()");
    }

    this->face.reset(face, FT_Done_Face);

    FT_F26Dot6 fontSize = this->size << 6;  // Size in 1/64th of a pixel
    if (FT_Set_Char_Size(this->face.get(), fontSize, 0, this->dpi, 0) != FT_Err_Ok) {
        throw std::runtime_error("FT_Set_Char_Size()");
    }
}

int Font::getDPI() const {
    return this->dpi;
}

int Font::getSize() const {
    return this->size;
}

const std::string& Font::getFilename() const {
    return this->filename;
}

std::shared_ptr<TextBitmap> Font::renderChar(char ch) {
    std::shared_ptr<CharGlyph> charGlyph = this->getCharGlyph(ch);
    if (charGlyph == nullptr) {
        return nullptr;
    }

    FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(charGlyph->record.get());
    FT_Bitmap charBitmap = bitmapGlyph->bitmap;

    std::shared_ptr<TextBitmap> textBitmap(new TextBitmap());
    textBitmap->width = charBitmap.width;
    textBitmap->height = charBitmap.rows;
    textBitmap->pixelDepth = 32;  // RGBA
    textBitmap->pixels = charGlyph->pixels;

    return textBitmap;
}

std::shared_ptr<TextBitmap> Font::renderString(const std::string& text) {
    FT_BBox stringBox = { };
    std::vector<std::shared_ptr<CharGlyph>> stringGlyphs;

    for (auto ch: text) {
        std::shared_ptr<CharGlyph> charGlyph = this->getCharGlyph(ch);
        if (charGlyph == nullptr) {
            continue;
        }

        stringBox.yMin = std::min(stringBox.yMin, charGlyph->box.yMin);
        stringBox.yMax = std::max(stringBox.yMax, charGlyph->box.yMax);
        stringBox.xMax += charGlyph->record->advance.x >> 16;  // 16.16 fixed float format

        stringGlyphs.push_back(charGlyph);
    }

    std::shared_ptr<TextBitmap> stringBitmap(new TextBitmap());
    stringBitmap->height = stringBox.yMax - stringBox.yMin;
    stringBitmap->width = stringBox.xMax;
    stringBitmap->pixelDepth = 32;  // RGBA

    int pixelBytes = stringBitmap->pixelDepth >> 3;
    int pixelsSize = stringBitmap->width * stringBitmap->height * pixelBytes;

    stringBitmap->pixels.reset(new char[pixelsSize]);
    char* pixels = stringBitmap->pixels.get();

    memset(pixels, 0, pixelsSize);
    int stringAdvance = 0;

    for (auto charGlyph: stringGlyphs) {
        FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(charGlyph->record.get());
        FT_Bitmap charBitmap = bitmapGlyph->bitmap;
        char* charPixels = charGlyph->pixels.get();

        int stringRow = stringBox.yMax - charGlyph->box.yMax;  // Character Y offset in string
        int charBitmapRowSize = charBitmap.width * pixelBytes;

        for (unsigned int charRow = 0; charRow < charBitmap.rows; charRow++, stringRow++) {
            int pixelsOffset = (stringRow * stringBitmap->width + stringAdvance) * pixelBytes;
            int charPixelsOffset = charRow * charBitmapRowSize;
            memcpy(pixels + pixelsOffset, charPixels + charPixelsOffset, charBitmapRowSize);
        }

        stringAdvance += charGlyph->record->advance.x >> 16;  // 16.16 fixed float format
    }

    return stringBitmap;
}

std::shared_ptr<Font::CharGlyph> Font::getCharGlyph(char ch) {
    auto charGlyphIt = this->charGlyphs.find(ch);
    if (charGlyphIt != this->charGlyphs.end()) {
        return charGlyphIt->second;
    }

    FT_UInt charIndex;
    if ((charIndex = FT_Get_Char_Index(this->face.get(), ch)) == 0) {
        return nullptr;
    }

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
    this->charGlyphs.emplace(ch, charGlyph);

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
