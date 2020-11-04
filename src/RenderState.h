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
#include <Object.h>
#include <Camera.h>
#include <Shader.h>
#include <memory>
#include <functional>

namespace Graphene {

class RenderState;
class RenderManager;

enum class RenderStateType { GEOMETRY, OVERLAY, BUFFER, SKYBOX, FRAME, SHADOWS, LIGHTS, NONE };
typedef std::function<void(RenderState* renderState, const std::shared_ptr<Object>)> RenderCallback;

class RenderState: public NonCopyable {
public:
    GRAPHENE_API virtual ~RenderState() = default;

    GRAPHENE_API void setShader(const std::shared_ptr<Shader> shader);
    GRAPHENE_API std::shared_ptr<Shader> getShader() const;

    GRAPHENE_API void setCallback(const RenderCallback& callback);
    GRAPHENE_API const RenderCallback& getCallback() const;

    GRAPHENE_API virtual void enter(RenderManager* renderManager);
    GRAPHENE_API virtual RenderStateType update(RenderManager* renderManager, const std::shared_ptr<Camera> camera) = 0;

protected:
    std::shared_ptr<Shader> shader;
    RenderCallback callback = [](RenderState* /*renderState*/, const std::shared_ptr<Object> /*object*/) { };
};

class RenderGeometry: public RenderState {
public:
    GRAPHENE_API RenderStateType update(RenderManager* renderManager, const std::shared_ptr<Camera> camera);
};

class RenderOverlay: public RenderGeometry { };
class RenderBuffer: public RenderGeometry { };

class RenderSkybox: public RenderState {
public:
    GRAPHENE_API RenderStateType update(RenderManager* renderManager, const std::shared_ptr<Camera> camera);
};

class RenderFrame: public RenderState {
public:
    GRAPHENE_API RenderStateType update(RenderManager* renderManager, const std::shared_ptr<Camera> camera);
};

class RenderShadows: public RenderState {
public:
    GRAPHENE_API RenderStateType update(RenderManager* renderManager, const std::shared_ptr<Camera> camera);
};

class RenderLights: public RenderState {
public:
    GRAPHENE_API RenderStateType update(RenderManager* renderManager, const std::shared_ptr<Camera> camera);
};

class RenderNone: public RenderState {
public:
    GRAPHENE_API RenderStateType update(RenderManager* renderManager, const std::shared_ptr<Camera> camera);
};

}  // namespace Graphene

#endif  // RENDERSTATE_H
