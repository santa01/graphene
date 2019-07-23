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

#ifndef FONT_H
#define FONT_H

#include <GrapheneApi.h>
#include <Image.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace Graphene {

class Font {
public:
    GRAPHENE_API Font(const std::string& filename, int size, int dpi = 72);

    GRAPHENE_API int getDPI() const;
    GRAPHENE_API int getSize() const;
    GRAPHENE_API const std::string& getFilename() const;

    GRAPHENE_API std::shared_ptr<Image> renderChar(wchar_t charCode);
    GRAPHENE_API std::shared_ptr<Image> renderString(const std::wstring& stringText);

private:
    typedef struct {
        FT_BBox box = { };
        std::shared_ptr<FT_GlyphRec> record;
        std::shared_ptr<char[]> pixels;
    } CharGlyph;

    std::shared_ptr<CharGlyph> getCharGlyph(FT_ULong charCode);

    std::string filename;
    int size;
    int dpi;

    std::shared_ptr<FT_FaceRec> face;
    std::unordered_map<FT_ULong, std::shared_ptr<CharGlyph>> charGlyphs;

private:
    static void initFreeType();

    typedef struct FT_LibraryRec_ FT_LibraryRec;
    static std::shared_ptr<FT_LibraryRec> library;
};

}  // namespace Graphene

#endif  // FONT_H
