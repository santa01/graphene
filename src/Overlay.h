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

#ifndef OVERLAY_H
#define OVERLAY_H

#include <GrapheneApi.h>
#include <Viewport.h>
#include <Layout.h>
#include <memory>

namespace Graphene {

class Overlay: public std::enable_shared_from_this<Overlay>, public Viewport {
public:
    GRAPHENE_API Overlay(int left, int top, int width, int height);

    GRAPHENE_API const std::shared_ptr<Layout>& getLayout() const;
    GRAPHENE_API void setLayout(const std::shared_ptr<Layout>& layout);

    GRAPHENE_API void update() override;

private:
    std::shared_ptr<Layout> layout;
};

}  // namespace Graphene

#endif  // OVERLAY_H
