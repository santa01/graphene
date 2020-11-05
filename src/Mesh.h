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

#ifndef MESH_H
#define MESH_H

#include <GrapheneApi.h>
#include <Material.h>
#include <Geometry.h>
#include <memory>

namespace Graphene {

class Mesh {
public:
    GRAPHENE_API Mesh(const std::shared_ptr<Geometry>& geometry);
    GRAPHENE_API Mesh(const std::shared_ptr<Material>& material, const std::shared_ptr<Geometry>& geometry);

    GRAPHENE_API const std::shared_ptr<Material>& getMaterial() const;
    GRAPHENE_API void setMaterial(const std::shared_ptr<Material>& material);

    GRAPHENE_API const std::shared_ptr<Geometry>& getGeometry() const;
    GRAPHENE_API void setGeometry(const std::shared_ptr<Geometry>& geometry);

    GRAPHENE_API void render();

private:
    std::shared_ptr<Material> material;
    std::shared_ptr<Geometry> geometry;
};

}  // namespace Graphene

#endif  // MESH_H
