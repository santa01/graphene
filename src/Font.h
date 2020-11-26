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
#include <RawImage.h>
#include <string>
#include <memory>
#include <unordered_map>

// Avoid FreeType requirement for Graphene dependent projects
typedef struct FT_LibraryRec_ FT_LibraryRec;
typedef struct FT_BBox_ FT_BBox;
typedef struct FT_GlyphRec_ FT_GlyphRec;
typedef struct FT_FaceRec_ FT_FaceRec;

namespace Graphene {

class Font {
public:
    GRAPHENE_API Font(const std::string& filename, int size, int dpi = 96);

    GRAPHENE_API const std::string& getFilename() const;
    GRAPHENE_API int getSize() const;
    GRAPHENE_API int getDPI() const;

    GRAPHENE_API void renderChar(wchar_t charCode, const std::shared_ptr<RawImage>& image);
    GRAPHENE_API void renderString(const std::wstring& stringText, const std::shared_ptr<RawImage>& image);

private:
    typedef struct {
        std::shared_ptr<FT_BBox> box;
        std::shared_ptr<FT_GlyphRec> record;
        std::shared_ptr<unsigned char[]> pixels;
    } CharGlyph;

    const std::shared_ptr<CharGlyph>& getCharGlyph(wchar_t charCode);

    std::string filename;
    int size;
    int dpi;

    std::shared_ptr<FT_FaceRec> face;
    std::unordered_map<wchar_t, std::shared_ptr<CharGlyph>> charGlyphs;

private:
    static void initFreeType();
    static std::shared_ptr<FT_LibraryRec> library;
};

}  // namespace Graphene

#endif  // FONT_H
