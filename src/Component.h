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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <memory>
#include <typeinfo>

namespace Graphene {

class Entity;

class Component: public NonCopyable {
public:
    GRAPHENE_API ~Component() = default;

    GRAPHENE_API const std::type_info& getType() const;

    template<typename T>
    bool isA() const { return typeid(T) == this->getType(); }

    template<typename T>
    T* toA() { return dynamic_cast<T*>(this); }

    template<typename T>
    std::shared_ptr<T> toShared() { return std::dynamic_pointer_cast<T>(this->shared_from_this()); }

    GRAPHENE_API const std::shared_ptr<Entity> getParent() const;

    GRAPHENE_API virtual void receiveEvent() = 0;
    GRAPHENE_API virtual void update() = 0;

protected:
    Component(const std::type_info& componentType);

    const std::type_info& componentType;

    friend class Entity;
    std::weak_ptr<Entity> parent;
};

}  // namespace Graphene

#endif  // COMPONENT_H
