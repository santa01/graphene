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
#if defined(_WIN32)
#include <WindowWin32.h>
#elif defined(__linux__)
#include <WindowLinux.h>
#endif
#include <chrono>
#include <iostream>
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

extern "C" void debugHandler(GLenum source, GLenum type, GLuint /*id*/, GLenum /*severity*/,
        GLsizei /*length*/, const GLchar* message, const GLvoid* /*userParam*/) {
    std::cout << typeMap[type] << " [" << sourceMap[source] << "]: "<< message << std::endl;
}

Engine::Engine(int width, int height):
        window(new WindowImpl(width, height)) {
    OpenGL::loadCore();
    OpenGL::loadExtensions();

    std::cout
            << "Vendor: " << glGetString(GL_VENDOR) << std::endl
            << "Renderer: " << glGetString(GL_RENDERER) << std::endl
            << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl
            << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    if (OpenGL::isExtensionSupported("GL_ARB_debug_output")) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
        glDebugMessageCallbackARB(debugHandler, nullptr);
    } else {
        std::cout << "GL_ARB_debug_output unavailable, OpenGL debug disabled" << std::endl;
    }

    glFrontFace(GL_CW);
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_CULL_FACE);

    this->sceneManagers.insert(std::make_shared<SceneManager>());
    this->objectManagers.insert(std::make_shared<ObjectManager>());

    this->window->onMouseMotion.connect(
            Signals::Slot<int, int>(&Engine::onMouseMotion, this, std::placeholders::_1, std::placeholders::_2));
    this->window->onMouseButton.connect(
            Signals::Slot<int, bool>(&Engine::onMouseButton, this, std::placeholders::_1, std::placeholders::_2));
    this->window->onKeyboardButton.connect(
            Signals::Slot<int, bool>(&Engine::onKeyboardButton, this, std::placeholders::_1, std::placeholders::_2));

    this->window->onQuit.connect(Signals::Slot<>(&Engine::onQuit, this));
    this->window->onIdle.connect(Signals::Slot<>(&Engine::onIdle, this));

    this->frameTime = 0.0f;
    this->running = true;
    this->result = 0;
}

int Engine::exec() {
    std::chrono::time_point<std::chrono::steady_clock> timestamp;

    while (this->running) {
        timestamp = std::chrono::steady_clock::now();
        this->window->dispatchEvents();

        for (auto& frameBuffer: this->frameBuffers) {
            if (frameBuffer->isAutoUpdate()) {
                frameBuffer->update();
            }
        }
        this->window->update();

        std::chrono::duration<float> duration = std::chrono::steady_clock::now() - timestamp;
        this->frameTime = duration.count();
    }

    return this->result;
}

}  // namespace Graphene
