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
#include <Camera.h>
#include <Shader.h>
#include <Mesh.h>
#include <utility>
#include <memory>
#include <functional>

#define GetRenderManager() RenderManager::getInstance()

namespace Graphene {

enum TextureUnits {
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_POSITION,
    TEXTURE_NORMAL,
    TEXTURE_DEPTH
};

enum class RenderStep { GEOMETRY, SKYBOX, FRAME, SHADOWS, LIGHTS, OVERLAY, BUFFER, NONE };
typedef std::function<void(const std::shared_ptr<Object>)> RenderCallback;

class RenderManager: public NonCopyable {
public:
    GRAPHENE_API static RenderManager& getInstance();

    GRAPHENE_API void setShadowPass(bool shadowPass);
    GRAPHENE_API bool hasShadowPass() const;

    GRAPHENE_API void setLightPass(bool lightPass);
    GRAPHENE_API bool hasLightPass() const;

    GRAPHENE_API void setShader(RenderStep step, std::shared_ptr<Shader> shader);
    GRAPHENE_API std::shared_ptr<Shader> getShader(RenderStep step) const;

    GRAPHENE_API void setRenderCallback(RenderStep step, RenderCallback callback);
    GRAPHENE_API const RenderCallback& getRenderCallback(RenderStep step) const;

    GRAPHENE_API void setRenderStep(RenderStep step);
    GRAPHENE_API RenderStep getRenderStep() const;

    GRAPHENE_API void render(const std::shared_ptr<Camera> camera);

private:
    RenderManager();

    void renderCallback(const std::shared_ptr<Object> object);

    RenderStep renderEntities(const std::shared_ptr<Camera> camera);
    RenderStep renderSkybox(const std::shared_ptr<Camera> camera);
    RenderStep renderFrame(const std::shared_ptr<Camera> camera);
    RenderStep renderShadows(const std::shared_ptr<Camera> camera);
    RenderStep renderLights(const std::shared_ptr<Camera> camera);

    std::unordered_map<RenderStep, std::shared_ptr<Shader>> shaders;
    std::unordered_map<RenderStep, RenderCallback> callbacks;

    typedef std::function<RenderStep(const std::shared_ptr<Camera>)> Renderer;
    std::unordered_map<RenderStep, Renderer> renderers;

    RenderStep step = RenderStep::NONE;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> frame;

    bool shadowPass = false;
    bool lightPass = false;
};

}  // namespace Graphene

#endif  // RENDERMANAGER_H
