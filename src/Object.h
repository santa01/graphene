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

#ifndef OBJECT_H
#define OBJECT_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <Rotatable.h>
#include <Movable.h>
#include <memory>
#include <string>
#include <typeinfo>

namespace Graphene {

class Scene;
class ObjectGroup;

class Object: public std::enable_shared_from_this<Object>, public Rotatable, public Movable, public NonCopyable {
public:
    GRAPHENE_API virtual ~Object() = default;

    GRAPHENE_API int getId() const;
    GRAPHENE_API const std::type_info& getType() const;

    template<typename T>
    bool isA() const { return typeid(T) == this->getType(); }

    template<typename T>
    T* toA() { return dynamic_cast<T*>(this); }

    template<typename T>
    std::shared_ptr<T> toShared() { return std::dynamic_pointer_cast<T>(this->shared_from_this()); }

    GRAPHENE_API const std::string& getName() const;
    GRAPHENE_API void setName(const std::string& objectName);

    GRAPHENE_API const std::shared_ptr<Scene> getScene() const;
    GRAPHENE_API const std::shared_ptr<ObjectGroup> getParent() const;

    GRAPHENE_API void targetAt(float x, float y, float z);
    GRAPHENE_API void targetAt(const Math::Vec3& vector);

protected:
    Object(const std::type_info& objectType);

    int objectId = 0;
    std::string objectName;
    const std::type_info& objectType;

    friend class Scene;
    std::weak_ptr<Scene> scene;

    friend class ObjectGroup;
    std::weak_ptr<ObjectGroup> parent;
};

}  // namespace Graphene

#endif  // OBJECT_H
