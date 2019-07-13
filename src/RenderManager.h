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
#include <memory>
#include <functional>
#include <stack>
#include <algorithm>
#include <string>

#define GetRenderManager() RenderManager::getInstance()

namespace Graphene {

enum TextureUnits {
    TEXTURE_DIFFUSE,
    TEXTURE_SPECULAR,
    TEXTURE_POSITION,
    TEXTURE_NORMAL,
    TEXTURE_DEPTH
};

typedef std::pair<std::string, std::function<void()>> RenderState;

class RenderManager: public NonCopyable {
public:
    GRAPHENE_API static RenderManager& getInstance();

    GRAPHENE_API std::shared_ptr<Shader> getGeometryShader();
    GRAPHENE_API void setGeometryShader(const std::shared_ptr<Shader> geometryShader);

    GRAPHENE_API std::shared_ptr<Shader> getAmbientShader();
    GRAPHENE_API void setAmbientShader(const std::shared_ptr<Shader> ambientShader);

    GRAPHENE_API std::shared_ptr<Shader> getLightingShader();
    GRAPHENE_API void setLightingShader(const std::shared_ptr<Shader> lightingShader);

    GRAPHENE_API bool hasShadowPass() const;
    GRAPHENE_API void setShadowPass(bool shadowPass);

    GRAPHENE_API bool hasLightPass() const;
    GRAPHENE_API void setLightPass(bool lightPass);

    GRAPHENE_API void pushState(const RenderState& state);
    GRAPHENE_API void popState();
    GRAPHENE_API void render(const std::shared_ptr<Camera> camera);

private:
    RenderManager();

    void renderShadows(const std::shared_ptr<Camera> camera);
    void renderLights(const std::shared_ptr<Camera> camera);

    std::shared_ptr<Shader> geometryShader;
    std::shared_ptr<Shader> ambientShader;
    std::shared_ptr<Shader> lightingShader;

    std::stack<RenderState> stateStack;
    std::shared_ptr<Mesh> frame;

    bool shadowPass = false;
    bool lightPass = false;
};

}  // namespace Graphene

#endif  // RENDERMANAGER_H
