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

#ifndef LABEL_H
#define LABEL_H

#include <GrapheneApi.h>
#include <Entity.h>
#include <ImageTexture.h>
#include <Font.h>
#include <string>
#include <memory>

namespace Graphene {

class Label: public Entity {
public:
    GRAPHENE_API Label(int width, int height, const std::shared_ptr<Font>& font);

    GRAPHENE_API int getWidth() const;
    GRAPHENE_API int getHeight() const;

    GRAPHENE_API void setFont(const std::shared_ptr<Font>& font);
    GRAPHENE_API const std::shared_ptr<Font>& getFont() const;

    GRAPHENE_API void setText(const std::wstring& text);
    GRAPHENE_API const std::wstring& getText() const;

private:
    void renderText();

    std::shared_ptr<ImageTexture> texture;
    std::shared_ptr<Font> font;
    std::wstring text;
};

}  // namespace Graphene

#endif  // LABEL_H
