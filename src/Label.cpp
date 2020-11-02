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

#include <Label.h>
#include <ObjectManager.h>
#include <Mesh.h>
#include <ImageTexture.h>
#include <RawImage.h>
#include <algorithm>
#include <cstring>

namespace Graphene {

Label::Label(int width, int height, const std::shared_ptr<Font> font):
        font(font) {
    auto mesh = GetObjectManager().createQuad(FaceWinding::WINDING_CLOCKWISE);
    this->addMesh(mesh);

    RawImage textureImage(width, height, 32);  // Data zeroed
    mesh->getMaterial()->setDiffuseTexture(std::make_shared<ImageTexture>(textureImage));
}

int Label::getWidth() const {
    auto mesh = *this->meshes.begin();
    return mesh->getMaterial()->getDiffuseTexture()->getWidth();
}

int Label::getHeight() const {
    auto mesh = *this->meshes.begin();
    return mesh->getMaterial()->getDiffuseTexture()->getHeight();
}

void Label::setFont(const std::shared_ptr<Font> font) {
    if (this->font != font) {
        this->font = font;
        this->renderText();
    }
}

const std::shared_ptr<Font> Label::getFont() const {
    return this->font;
}

void Label::setText(const std::wstring& text) {
    if (this->text != text) {
        this->text = text;
        this->renderText();
    }
}

const std::wstring& Label::getText() const {
    return this->text;
}

void Label::renderText() {
    auto textBitmap = this->font->renderString(this->text);
    RawImage textureImage(this->getWidth(), this->getHeight(), textBitmap->getPixelDepth());

    int minWidth = std::min<int>(textureImage.getWidth(), textBitmap->getWidth());
    int minHeight = std::min<int>(textureImage.getHeight(), textBitmap->getHeight());

    int pixelBytes = textBitmap->getPixelDepth() >> 3;
    int minRowSize = minWidth * pixelBytes;
    int imageRowSize = textureImage.getWidth() * pixelBytes;
    int textRowSize = textBitmap->getWidth() * pixelBytes;

    char* imagePixels = textureImage.getRawData();
    const char* textPixels = reinterpret_cast<const char*>(textBitmap->getPixels());

    for (int textRow = 0; textRow < minHeight; textRow++) {
        int imageRowOffset = textRow * imageRowSize;
        int textRowOffset = (minHeight - textRow - 1) * textRowSize;
        memcpy(imagePixels + imageRowOffset, textPixels + textRowOffset, minRowSize);
    }

    auto mesh = *this->meshes.begin();
    auto texture = std::dynamic_pointer_cast<ImageTexture>(mesh->getMaterial()->getDiffuseTexture());

    texture->update(textureImage);
}

}  // namespace Graphene
