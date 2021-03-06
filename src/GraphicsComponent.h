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

#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

#include <GrapheneApi.h>
#include <MetaObject.h>
#include <Component.h>
#include <Material.h>
#include <Mesh.h>
#include <vector>
#include <memory>

namespace Graphene {

class GraphicsComponent: public MetaObject<GraphicsComponent>, public Component {
public:
    GRAPHENE_API GraphicsComponent();

    GRAPHENE_API const std::vector<std::shared_ptr<Material>>& getMaterials() const;
    GRAPHENE_API const std::vector<std::shared_ptr<Mesh>>& getMeshes() const;

    GRAPHENE_API void addGraphics(const std::shared_ptr<Material>& material, const std::shared_ptr<Mesh>& mesh);
    GRAPHENE_API void render();

    GRAPHENE_API void receiveEvent(const std::shared_ptr<ComponentEvent>& event) override;
    GRAPHENE_API void update(float /*deltaTime*/) override { };

private:
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Mesh>> meshes;
};

}  // namespace Graphene

#endif  // GRAPHICSCOMPONENT_H
