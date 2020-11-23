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

#include <TextComponent.h>
#include <MetaObject.h>
#include <Entity.h>
#include <algorithm>
#include <cstring>
#include <cassert>

namespace Graphene {

TextComponent::TextComponent(int width, int height, const std::shared_ptr<Font>& font):
        Component(TextComponent::ID),
        width(width),
        height(height),
        font(font) {
}

int TextComponent::getWidth() const {
    return this->width;
}

int TextComponent::getHeight() const {
    return this->height;
}

const std::shared_ptr<RawImage>& TextComponent::getTextImage() const {
    return this->textImage;
}

void TextComponent::setFont(const std::shared_ptr<Font>& font) {
    this->font = font;
    this->parametersDirty = true;
}

const std::shared_ptr<Font>& TextComponent::getFont() const {
    return this->font;
}

void TextComponent::setText(const std::wstring& text) {
    this->text = text;
    this->parametersDirty = true;
}

const std::wstring& TextComponent::getText() const {
    return this->text;
}

void TextComponent::update(float /*deltaTime*/) {
    if (this->parametersDirty) {
        this->parametersDirty = false;
        this->renderText();

        auto event = std::make_shared<TextureUpdateEvent>();
        event->setImage(this->textImage);

        auto entity = this->getParent();
        entity->sendEvent(event);
    }
}

void TextComponent::renderText() {
    auto textBitmap = this->font->renderString(this->text);
    this->textImage = std::make_shared<RawImage>(this->width, this->height, textBitmap->getPixelDepth());

    int minWidth = std::min<int>(this->width, textBitmap->getWidth());
    int minHeight = std::min<int>(this->height, textBitmap->getHeight());

    int pixelBytes = textBitmap->getPixelDepth() >> 3;
    int minRowSize = minWidth * pixelBytes;
    int imageRowSize = this->width * pixelBytes;
    int textRowSize = textBitmap->getWidth() * pixelBytes;

    char* imagePixels = this->textImage->getRawData();
    const char* textPixels = reinterpret_cast<const char*>(textBitmap->getPixels());

    for (int textRow = 0; textRow < minHeight; textRow++) {
        int imageRowOffset = textRow * imageRowSize;
        int textRowOffset = (minHeight - textRow - 1) * textRowSize;

        assert(imageRowOffset + minRowSize < this->textImage->getPixelsSize());
        std::memcpy(imagePixels + imageRowOffset, textPixels + textRowOffset, minRowSize);
    }
}

}  // namespace Graphene
