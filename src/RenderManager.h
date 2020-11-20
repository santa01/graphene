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

#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <RenderState.h>
#include <MetaObject.h>
#include <Camera.h>
#include <Mesh.h>
#include <unordered_map>
#include <memory>

#define GetRenderManager() RenderManager::getInstance()

namespace Graphene {

class RenderManager: public NonCopyable {
public:
    GRAPHENE_API static RenderManager& getInstance();

    GRAPHENE_API void setShadowPass(bool shadowPass);
    GRAPHENE_API bool hasShadowPass() const;

    GRAPHENE_API void setLightPass(bool lightPass);
    GRAPHENE_API bool hasLightPass() const;

    GRAPHENE_API const std::shared_ptr<Mesh>& getFrame() const;

    GRAPHENE_API void setRenderState(MetaType stateType);
    GRAPHENE_API const std::shared_ptr<RenderState>& getRenderState(MetaType stateType) const;

    GRAPHENE_API void update(const std::shared_ptr<Camera>& camera);

    GRAPHENE_API void teardown();

private:
    RenderManager();

    bool shadowPass = false;
    bool lightPass = false;

    std::shared_ptr<Mesh> frame;

    std::unordered_map<MetaIndex, std::shared_ptr<RenderState>> renderStates;
    std::shared_ptr<RenderState> renderState;
};

}  // namespace Graphene

#endif  // RENDERMANAGER_H
