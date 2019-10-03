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
#include <Mesh.h>
#include <Object.h>
#include <unordered_set>
#include <memory>

namespace Graphene {

class Entity: public Object, public Scalable {
public:
    GRAPHENE_API Entity();
    GRAPHENE_API virtual ~Entity() = default;

    GRAPHENE_API void setVisible(bool visible);
    GRAPHENE_API bool isVisible() const;
    GRAPHENE_API bool isSkybox() const;

    GRAPHENE_API const std::unordered_set<std::shared_ptr<Mesh>>& getMeshes() const;
    GRAPHENE_API void addMesh(const std::shared_ptr<Mesh> mesh);

protected:
    bool visible = true;
    bool skybox = false;

    std::unordered_set<std::shared_ptr<Mesh>> meshes;
};

}  // namespace Graphene

#endif  // ENTITY_H
