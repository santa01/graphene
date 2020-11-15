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

#ifndef OBJECTGROUP_H
#define OBJECTGROUP_H

#include <GrapheneApi.h>
#include <Scalable.h>
#include <Object.h>
#include <vector>
#include <memory>

namespace Graphene {

class ObjectGroup: public std::enable_shared_from_this<ObjectGroup>, public Object, public Scalable {
public:
    GRAPHENE_API ObjectGroup();

    GRAPHENE_API const std::vector<std::shared_ptr<Object>>& getObjects() const;
    GRAPHENE_API void addObject(const std::shared_ptr<Object>& object);

private:
    std::vector<std::shared_ptr<Object>> objects;
};

}  // namespace Graphene

#endif  // OBJECTGROUP_H
