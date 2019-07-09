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
#include <Rotatable.h>
#include <Movable.h>
#include <memory>

namespace Graphene {

class SceneNode;

enum class ObjectType { ENTITY, LIGHT, CAMERA };

class Object: public Rotatable, public Movable {
public:
    GRAPHENE_API Object(ObjectType objectType);
    GRAPHENE_API virtual ~Object() = default;

    GRAPHENE_API ObjectType getType() const;
    GRAPHENE_API std::shared_ptr<class SceneNode> getParent();

    GRAPHENE_API void targetAt(float x, float y, float z);
    GRAPHENE_API void targetAt(const Math::Vec3& vector);

private:
    ObjectType objectType;

    friend class SceneNode;
    std::weak_ptr<class SceneNode> parent;
};

}  // namespace Graphene

#endif  // OBJECT_H
