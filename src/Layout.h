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

#ifndef LAYOUT_H
#define LAYOUT_H

#include <GrapheneApi.h>
#include <Label.h>
#include <memory>
#include <unordered_map>
#include <utility>

namespace Graphene {

class Overlay;

class Layout {
public:
    GRAPHENE_API std::shared_ptr<class Overlay> getOverlay() const;

    GRAPHENE_API void addComponent(const std::shared_ptr<Label> component, int x, int y);
    GRAPHENE_API void removeComponent(const std::shared_ptr<Label> component);

    GRAPHENE_API void arrangeComponents();

private:
    std::unordered_map<std::shared_ptr<Label>, std::pair<int, int>> componentPositions;
    std::unordered_map<std::shared_ptr<Label>, std::pair<float, float>> componentScaleFactors;

    friend class Overlay;
    std::weak_ptr<class Overlay> overlay;
};

}  // namespace Graphene

#endif  // LAYOUT_H
