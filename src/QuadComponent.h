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

#ifndef QUADCOMPONENT_H
#define QUADCOMPONENT_H

#include <GrapheneApi.h>
#include <MetaObject.h>
#include <Component.h>
#include <ComponentEvent.h>
#include <utility>
#include <memory>

namespace Graphene {

typedef std::pair<int, int> QuadOrigin;

class QuadComponent: public MetaObject<QuadComponent>, public Component {
public:
    GRAPHENE_API QuadComponent(int width, int height);

    GRAPHENE_API int getWidth() const;
    GRAPHENE_API void setWidth(int width);

    GRAPHENE_API int getHeight() const;
    GRAPHENE_API void setHeight(int height);

    GRAPHENE_API void setOrigin(int x, int y);
    GRAPHENE_API void setOrigin(const QuadOrigin& origin);
    GRAPHENE_API const QuadOrigin& getOrigin() const;

    GRAPHENE_API void receiveEvent(const std::shared_ptr<ComponentEvent>& /*event*/) override { }
    GRAPHENE_API void update(float /*deltaTime*/) override { }

private:
    int width = 0;
    int height = 0;

    QuadOrigin origin = { 0, 0 };
};

}  // namespace Graphene

#endif  // QUADCOMPONENT_H
