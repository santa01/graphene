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

#ifndef METAOBJECT_H
#define METAOBJECT_H

#include <GrapheneApi.h>
#include <typeinfo>
#include <typeindex>
#include <memory>

namespace Graphene {

typedef const std::type_info& MetaType;
typedef std::type_index MetaIndex;

class MetaBase: public std::enable_shared_from_this<MetaBase> {
public:
    GRAPHENE_API virtual ~MetaBase() = default;

    GRAPHENE_API MetaType getType() const;

    template<typename T>
    bool isA() const;

    template<typename T>
    std::shared_ptr<T> toA();

protected:
    MetaBase(MetaType objectType);

    MetaType objectType;
};

template<typename T>
class MetaObject {
public:
    static MetaType ID;
    static MetaIndex INDEX;
};

template<typename T>
bool MetaBase::isA() const { return MetaObject<T>::ID == this->getType(); }

template<typename T>
std::shared_ptr<T> MetaBase::toA() { return std::dynamic_pointer_cast<T>(this->shared_from_this()); }

template<typename T>
MetaType MetaObject<T>::ID = typeid(T);

template<typename T>
MetaIndex MetaObject<T>::INDEX = MetaIndex(MetaObject<T>::ID);

}  // namespace Graphene

#endif  // METAOBJECT_H
