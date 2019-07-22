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

#include <Engine.h>
#include <OpenGL.h>
#include <Logger.h>
#include <ObjectManager.h>
#include <RenderManager.h>
#include <EngineConfig.h>
#include <Shader.h>
#if defined(_WIN32)
#include <Win32Window.h>
#elif defined(__linux__)
#include <LinuxWindow.h>
#endif
#include <chrono>
#include <thread>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace Graphene {

std::unordered_map<GLenum, std::string> sourceMap = {
    { GL_DEBUG_SOURCE_API_ARB,             "OpenGL" },
    { GL_DEBUG_SOURCE_SHADER_COMPILER_ARB, "GLSL" },
    { GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB,   "Window system" },
    { GL_DEBUG_SOURCE_THIRD_PARTY_ARB,     "3rd party" },
    { GL_DEBUG_SOURCE_APPLICATION_ARB,     "Application" },
    { GL_DEBUG_SOURCE_OTHER_ARB,           "Other" }
};

std::unordered_map<GLenum, std::string> typeMap = {
    { GL_DEBUG_TYPE_ERROR_ARB,               "Error" },
    { GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB, "Deprecated" },
    { GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB,  "Indefined" },
    { GL_DEBUG_TYPE_PERFORMANCE_ARB,         "Performance" },
    { GL_DEBUG_TYPE_PORTABILITY_ARB,         "Portability" },
    { GL_DEBUG_TYPE_OTHER_ARB,               "Other" }
};

void debugHandler(GLenum source, GLenum type, GLuint /*id*/, GLenum /*severity*/,
        GLsizei /*length*/, const GLchar* message, const GLvoid* /*userParam*/) {
    LogDebug("%s [%s]: %s", sourceMap[source].c_str(), typeMap[type].c_str(), message);
}

Engine::Engine() {
    if (GetEngineConfig().isDebug()) {
        GetLogger().setLogLevel(LogLevel::LOG_DEBUG);
    }
}

const std::unordered_set<std::shared_ptr<FrameBuffer>>& Engine::getFrameBuffers() const {
    return this->frameBuffers;
}

std::shared_ptr<FrameBuffer> Engine::createFrameBuffer(int width, int height) {
    auto frameBuffer = std::make_shared<FrameBuffer>(width, height);
    this->frameBuffers.insert(frameBuffer);

    return frameBuffer;
}

const std::unordered_set<std::shared_ptr<Scene>>& Engine::getScenes() const {
    return this->scenes;
}

std::shared_ptr<Scene> Engine::createScene() {
    auto scene = std::make_shared<Scene>();
    this->scenes.insert(scene);

    return scene;
}

std::shared_ptr<Window> Engine::getWindow() {
    return this->window;
}

float Engine::getFrameTime() const {
    return this->frameTime;
}

void Engine::exit(int result) {
    this->result = result;
    this->running = false;
}

int Engine::exec() {
    std::chrono::time_point<std::chrono::steady_clock> timestamp;

    this->setupWindow();
    this->setupOpenGL();
    this->setupEngine();
    this->onSetupSignal();

    while (this->running) {
        timestamp = std::chrono::steady_clock::now();

        if (this->window->dispatchEvents()) {
            this->onQuitSignal();
            this->exit(0);
        } else {
            this->onIdleSignal();
            this->update();
        }

        std::chrono::duration<float> duration = std::chrono::steady_clock::now() - timestamp;
        this->frameTime = duration.count();

        float maxFrameTime = 1.0f / GetEngineConfig().getMaxFps();
        if (this->frameTime < maxFrameTime) {
            std::this_thread::sleep_for(std::chrono::duration<float>(maxFrameTime - this->frameTime));
            this->frameTime = maxFrameTime;
        }

        this->renderTime += this->getFrameTime();
        this->frames++;

        if (renderTime >= 1.0f) {
            float fps = this->frames / this->renderTime;
            Graphene::LogInfo("%d frames in %.1f seconds = %.3f fps", this->frames, this->renderTime, fps);

            this->renderTime = 0.0f;
            this->frames = 0;
        }
    }

    GetObjectManager().clearCache();
    return this->result;
}

void Engine::setupWindow() {
    auto& config = GetEngineConfig();

#if defined(_WIN32)
    this->window = std::shared_ptr<Window>(new Win32Window(config.getWidth(), config.getHeight()));
#elif defined(__linux__)
    this->window = std::shared_ptr<Window>(new LinuxWindow(config.getWidth(), config.getHeight()));
#endif

    this->window->onMouseMotionSignal.connect(
            Signals::Slot<int, int>(&Engine::onMouseMotion, this, std::placeholders::_1, std::placeholders::_2));
    this->window->onMouseButtonSignal.connect(
            Signals::Slot<int, bool>(&Engine::onMouseButton, this, std::placeholders::_1, std::placeholders::_2));
    this->window->onKeyboardButtonSignal.connect(
            Signals::Slot<int, bool>(&Engine::onKeyboardButton, this, std::placeholders::_1, std::placeholders::_2));
}

void Engine::setupOpenGL() {
    OpenGL::loadCore();
    OpenGL::loadExtensions();

    LogInfo("Vendor: %s", glGetString(GL_VENDOR));
    LogInfo("Renderer: %s", glGetString(GL_RENDERER));
    LogInfo("OpenGL Version: %s", glGetString(GL_VERSION));
    LogInfo("GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    if (GetEngineConfig().isDebug()) {
        if (OpenGL::isExtensionSupported("GL_ARB_debug_output")) {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
            glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
            glDebugMessageCallbackARB(debugHandler, nullptr);
        } else {
            LogWarn("GL_ARB_debug_output unavailable, OpenGL debug disabled");
        }
    }

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
}

void Engine::setupEngine() {
    auto& renderManager = GetRenderManager();

    auto geometryOutputShader = std::make_shared<Shader>("shaders/geometry_output.shader");
    renderManager.setShader(RenderStep::GEOMETRY, geometryOutputShader);

    auto ambientLightingShader = std::make_shared<Shader>("shaders/ambient_lighting.shader");
    renderManager.setShader(RenderStep::FRAME, ambientLightingShader);

    auto overlayOutputShader = std::make_shared<Shader>("shaders/overlay_output.shader");
    renderManager.setShader(RenderStep::OVERLAY, overlayOutputShader);

    auto deferredLightingShader = std::make_shared<Shader>("shaders/deferred_lighting.shader");
    renderManager.setShader(RenderStep::LIGHTS, deferredLightingShader);

    this->onSetupSignal.connect(Signals::Slot<>(&Engine::onSetup, this));
    this->onIdleSignal.connect(Signals::Slot<>(&Engine::onIdle, this));
    this->onQuitSignal.connect(Signals::Slot<>(&Engine::onQuit, this));
}

void Engine::update() {
    for (auto& frameBuffer: this->frameBuffers) {
        frameBuffer->update();
    }

    this->window->update();
}

}  // namespace Graphene
