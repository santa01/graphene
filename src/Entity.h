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

#ifndef ENTITY_H
#define ENTITY_H

#include <GrapheneApi.h>
#include <Scalable.h>
#include <Component.h>
#include <ComponentEvent.h>
#include <MetaObject.h>
#include <Object.h>
#include <vector>
#include <memory>

namespace Graphene {

class Entity: public MetaObject<Entity>, public Object, public Scalable {
public:
    GRAPHENE_API Entity();

    GRAPHENE_API void setVisible(bool visible);
    GRAPHENE_API bool isVisible() const;

    template<typename T>
    std::shared_ptr<T> getComponent() const {
        auto predicate = [](const std::shared_ptr<Component>& component) { return component->isA<T>(); };
        auto componentIt = std::find_if(this->components.begin(), this->components.end(), predicate);
        return (componentIt != this->components.end()) ? (*componentIt)->toA<T>() : nullptr;
    }

    GRAPHENE_API const std::vector<std::shared_ptr<Component>>& getComponents() const;
    GRAPHENE_API void addComponent(const std::shared_ptr<Component>& component);

    GRAPHENE_API void sendEvent(const std::shared_ptr<ComponentEvent>& event) const;
    GRAPHENE_API void update() const;

protected:
    bool visible = true;

    std::vector<std::shared_ptr<Component>> components;
};

}  // namespace Graphene

#endif  // ENTITY_H
