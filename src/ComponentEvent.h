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

#ifndef COMPONENTEVENT_H
#define COMPONENTEVENT_H

#include <GrapheneApi.h>
#include <MetaObject.h>
#include <Image.h>
#include <memory>

namespace Graphene {

class ComponentEvent: public std::enable_shared_from_this<ComponentEvent> {
public:
    GRAPHENE_API virtual ~ComponentEvent() = default;

    GRAPHENE_API MetaType getType() const;

    template<typename T>
    bool isA() const { return MetaObject<T>::ID == this->getType(); }

    template<typename T>
    std::shared_ptr<T> toA() { return std::dynamic_pointer_cast<T>(this->shared_from_this()); }

protected:
    ComponentEvent(MetaType eventType);

    MetaType eventType;
};

class TextureUpdateEvent: public MetaObject<TextureUpdateEvent>, public ComponentEvent {
public:
    GRAPHENE_API TextureUpdateEvent();

    GRAPHENE_API const std::shared_ptr<Image>& getImage() const;
    GRAPHENE_API void setImage(const std::shared_ptr<Image>& image);

private:
    std::shared_ptr<Image> image;
};

}  // namespace Graphene

#endif  // COMPONENTEVENT_H
