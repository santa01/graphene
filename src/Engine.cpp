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
#if defined(_WIN32)
#include <WindowWin32.h>
#elif defined(__linux__)
#include <WindowLinux.h>
#endif
#include <chrono>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace Graphene {

#if defined(_WIN32)
typedef WindowWin32 WindowImpl;
#elif defined(__linux__)
typedef WindowLinux WindowImpl;
#endif

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
    if (this->config.isDebug()) {
        GetLogger().setLogLevel(LogLevel::LOG_DEBUG);
    }
}

Engine::Engine(const EngineConfig& config):
        config(config) {
    if (this->config.isDebug()) {
        GetLogger().setLogLevel(LogLevel::LOG_DEBUG);
    }
}

const EngineConfig& Engine::getConfig() const {
    return this->config;
}

const std::unordered_set<std::shared_ptr<FrameBuffer>>& Engine::getFrameBuffers() const {
    return this->frameBuffers;
}

void Engine::addFrameBuffer(const std::shared_ptr<FrameBuffer> frameBuffer) {
    if (frameBuffer == nullptr) {
        throw std::invalid_argument(LogFormat("FrameBuffer cannot be nullptr"));
    }

    this->frameBuffers.insert(frameBuffer);
}

const std::unordered_set<std::shared_ptr<SceneManager>>& Engine::getSceneManagers() const {
    return this->sceneManagers;
}

void Engine::addSceneManager(const std::shared_ptr<SceneManager> sceneManager) {
    if (sceneManager == nullptr) {
        throw std::invalid_argument(LogFormat("SceneManager cannot be nullptr"));
    }

    this->sceneManagers.insert(sceneManager);
}

const std::unordered_set<std::shared_ptr<ObjectManager>>& Engine::getObjectManagers() const {
    return this->objectManagers;
}

void Engine::addObjectManager(const std::shared_ptr<ObjectManager> objectManager) {
    if (objectManager == nullptr) {
        throw std::invalid_argument(LogFormat("ObjectManager cannot be nullptr"));
    }

    this->objectManagers.insert(objectManager);
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
            this->render();
        }

        std::chrono::duration<float> duration = std::chrono::steady_clock::now() - timestamp;
        this->frameTime = duration.count();
    }

    return this->result;
}

void Engine::setupWindow() {
    this->window = std::shared_ptr<Window>(
            new WindowImpl(this->config.getWidth(), this->config.getHeight()));

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

    if (this->config.isDebug()) {
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
}

void Engine::setupEngine() {
    this->sceneManagers.insert(std::make_shared<SceneManager>());
    this->objectManagers.insert(std::make_shared<ObjectManager>());

    this->onSetupSignal.connect(Signals::Slot<>(&Engine::onSetup, this));
    this->onIdleSignal.connect(Signals::Slot<>(&Engine::onIdle, this));
    this->onQuitSignal.connect(Signals::Slot<>(&Engine::onQuit, this));
}

void Engine::render() {
    for (auto& frameBuffer: this->frameBuffers) {
        if (frameBuffer->isAutoUpdate()) {
            frameBuffer->update();
        }
    }

    this->window->update();
}

}  // namespace Graphene
