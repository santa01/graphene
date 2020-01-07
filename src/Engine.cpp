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
#include <Logger.h>
#include <ObjectManager.h>
#include <RenderManager.h>
#include <EngineConfig.h>
#if defined(_WIN32)
#include <Win32Window.h>
#elif defined(__linux__)
#include <LinuxWindow.h>
#endif
#include <chrono>
#include <thread>
#include <string>
#include <sstream>
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
    { GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB,  "Undefined" },
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

std::shared_ptr<FrameBuffer> Engine::createFrameBuffer(int width, int height, GLenum format) {
    auto frameBuffer = std::make_shared<FrameBuffer>(width, height, format);
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

std::shared_ptr<Window> Engine::getWindow() const {
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

    auto& config = GetEngineConfig();
    LogInfo("Engine configuration:\n%s", config.toString().c_str());

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
        float maxFps = config.getMaxFps();

        if (!config.isVsync() && maxFps > 0.0f) {
            float maxFrameTime = 1.0f / maxFps;

            if (this->frameTime < maxFrameTime) {
                std::this_thread::sleep_for(std::chrono::duration<float>(maxFrameTime - this->frameTime));
                this->frameTime = maxFrameTime;
            }
        }

        this->renderTime += this->getFrameTime();
        this->frames++;

        if (this->renderTime >= 1.0f) {
            float fps = this->frames / this->renderTime;
            LogInfo("%d frames in %.1f seconds = %.3f fps", this->frames, this->renderTime, fps);

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
    this->window->setVsync(config.isVsync());
    this->window->setFullscreen(config.isFullscreen());

    this->window->onMouseMotionSignal.connect(
            Signals::Slot<int, int>(&Engine::onMouseMotion, this, std::placeholders::_1, std::placeholders::_2));
    this->window->onMouseButtonSignal.connect(
            Signals::Slot<MouseButton, bool>(&Engine::onMouseButton, this, std::placeholders::_1, std::placeholders::_2));
    this->window->onKeyboardKeySignal.connect(
            Signals::Slot<KeyboardKey, bool>(&Engine::onKeyboardKey, this, std::placeholders::_1, std::placeholders::_2));
}

void Engine::setupOpenGL() {
    OpenGL::loadCore();
    OpenGL::loadExtensions();

    LogInfo("OpenGL vendor: %s", glGetString(GL_VENDOR));
    LogInfo("OpenGL renderer: %s", glGetString(GL_RENDERER));
    LogInfo("OpenGL version: %s", glGetString(GL_VERSION));
    LogInfo("GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    std::ostringstream openglExtensions;
    for (auto& extension: OpenGL::getSupportedExtensions()) {
        openglExtensions << "\n" << extension;
    }

    std::ostringstream platformExtensions;
    for (auto& extension: this->window->getSupportedExtensions()) {
        platformExtensions << "\n" << extension;
    }

    LogTrace("OpenGL extensions: %s", openglExtensions.str().c_str());
    LogTrace("Platform extensions: %s", platformExtensions.str().c_str());

    if (GetEngineConfig().isDebug()) {
        if (OpenGL::isExtensionSupported("GL_ARB_debug_output")) {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
            glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
            glDebugMessageCallbackARB(debugHandler, nullptr);
        } else {
            LogWarn("GL_ARB_debug_output unavailable, OpenGL debug disabled");
        }
    }

    if (OpenGL::isExtensionSupported("GL_ARB_seamless_cube_map")) {
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    } else {
        LogWarn("GL_ARB_seamless_cube_map unavailable, skybox may expose seams across faces");
    }

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    // No visual effect for 32bit color bit context, not supported for non RGB frame buffers
    glDisable(GL_DITHER);

    // Skybox rendering, see skybox_output.shader
    glDepthFunc(GL_LEQUAL);
}

void Engine::setupEngine() {
    auto& renderManager = GetRenderManager();
    auto& objectManager = GetObjectManager();

    renderManager.setShader(RenderStep::GEOMETRY, objectManager.createShader("shaders/geometry_output.shader"));
    renderManager.setShader(RenderStep::SKYBOX, objectManager.createShader("shaders/skybox_output.shader"));
    renderManager.setShader(RenderStep::FRAME, objectManager.createShader("shaders/ambient_lighting.shader"));
    renderManager.setShader(RenderStep::LIGHTS, objectManager.createShader("shaders/deferred_lighting.shader"));
    renderManager.setShader(RenderStep::OVERLAY, objectManager.createShader("shaders/overlay_output.shader"));

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
