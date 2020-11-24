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
#include <cassert>

namespace Graphene {

TextComponent::TextComponent(int width, int height, const std::shared_ptr<Font>& font):
        Component(TextComponent::ID),
        width(width),
        height(height),
        font(font) {
    this->textImage = std::make_shared<RawImage>(this->width, this->height, 32);
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

const Math::Vec3& TextComponent::getColor() const {
    return this->color;
}

void TextComponent::setColor(const Math::Vec3& color) {
    this->color = color;
    this->parametersDirty = true;
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
    unsigned char colorPixel[] = { 255, 255, 255, 0 };  // RGBA
    colorPixel[0] = static_cast<unsigned char>(colorPixel[0] * this->color.get(Math::Vec3::X));
    colorPixel[1] = static_cast<unsigned char>(colorPixel[1] * this->color.get(Math::Vec3::Y));
    colorPixel[2] = static_cast<unsigned char>(colorPixel[2] * this->color.get(Math::Vec3::Z));

    this->textImage->clear(colorPixel, 32);
    this->font->renderString(this->text, this->textImage);
}

}  // namespace Graphene
