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

#ifndef RENDERSTATE_H
#define RENDERSTATE_H

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <MetaObject.h>
#include <Object.h>
#include <Camera.h>
#include <Shader.h>
#include <memory>
#include <functional>

namespace Graphene {

class RenderState;
class RenderManager;

typedef std::function<void(RenderState* renderState, const std::shared_ptr<Object>)> RenderStateCallback;

class RenderState: public NonCopyable {
public:
    GRAPHENE_API virtual ~RenderState() = default;

    GRAPHENE_API void setShader(const std::shared_ptr<Shader>& shader);
    GRAPHENE_API const std::shared_ptr<Shader>& getShader() const;

    GRAPHENE_API void setCallback(const RenderStateCallback& callback);
    GRAPHENE_API const RenderStateCallback& getCallback() const;

    GRAPHENE_API virtual void enter(RenderManager* renderManager);
    GRAPHENE_API virtual MetaType update(RenderManager* renderManager, const std::shared_ptr<Camera>& camera);

protected:
    std::shared_ptr<Shader> shader;
    RenderStateCallback callback = [](RenderState* /*renderState*/, const std::shared_ptr<Object>& /*object*/) { };
};

class RenderGeometry: public MetaObject<RenderGeometry>, public RenderState { };
class RenderOverlay: public MetaObject<RenderOverlay>, public RenderState { };
class RenderBuffer: public MetaObject<RenderBuffer>, public RenderState { };

class RenderSkybox: public MetaObject<RenderSkybox>, public RenderState {
public:
    GRAPHENE_API MetaType update(RenderManager* renderManager, const std::shared_ptr<Camera>& camera) override;
};

class RenderFrame: public MetaObject<RenderFrame>, public RenderState {
public:
    GRAPHENE_API MetaType update(RenderManager* renderManager, const std::shared_ptr<Camera>& camera) override;
};

class RenderShadows: public MetaObject<RenderShadows>, public RenderState {
public:
    GRAPHENE_API MetaType update(RenderManager* renderManager, const std::shared_ptr<Camera>& camera) override;
};

class RenderLights: public MetaObject<RenderLights>, public RenderState {
public:
    GRAPHENE_API MetaType update(RenderManager* renderManager, const std::shared_ptr<Camera>& camera) override;
};

class RenderNone: public MetaObject<RenderNone>, public RenderState {
public:
    GRAPHENE_API MetaType update(RenderManager* renderManager, const std::shared_ptr<Camera>& camera) override;
};

}  // namespace Graphene

#endif  // RENDERSTATE_H
