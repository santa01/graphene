#include <Engine.h>
#include <OpenGL.h>
#include <iostream>
#include <string>
#include <unordered_map>

namespace Graphene {

const std::unordered_map<GLenum, std::string> debugSource = {
    { GL_DEBUG_SOURCE_API_ARB,             "OpenGL" },
    { GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB,   "Window System" },
    { GL_DEBUG_SOURCE_SHADER_COMPILER_ARB, "GLSL" },
    { GL_DEBUG_SOURCE_THIRD_PARTY_ARB,     "3rd Party" },
    { GL_DEBUG_SOURCE_APPLICATION_ARB,     "Application" },
    { GL_DEBUG_SOURCE_OTHER_ARB,           "Other" }
};

const std::unordered_map<GLenum, std::string> debugType = {
    { GL_DEBUG_TYPE_ERROR_ARB,               "Error" },
    { GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB, "Deprecated" },
    { GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB,  "Undefined" },
    { GL_DEBUG_TYPE_PORTABILITY_ARB,         "Portability" },
    { GL_DEBUG_TYPE_PERFORMANCE_ARB,         "Performance" },
    { GL_DEBUG_TYPE_OTHER_ARB,               "Other" }
};

extern "C" void debugHandler(
        GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
    std::cout << debugType.at(type) << " [" << debugSource.at(source) << "]: " << message << std::endl;
}

Engine::Engine(int width, int height, HINSTANCE instance):
        window(width, height, instance) {
    this->window.createRenderingContext();
    OpenGL::loadExtensions();
    this->window.destroyRenderingContext();

    this->window.createRenderingContextARB();

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    OpenGL::glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
    OpenGL::glDebugMessageCallbackARB(debugHandler, nullptr);

    std::cout
        << "Vendor: " << glGetString(GL_VENDOR) << std::endl
        << "Renderer: " << glGetString(GL_RENDERER) << std::endl
        << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl
        << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    this->window.show();
}

int Engine::run() {
    this->window.processEvents(false);
    this->window.destroyRenderingContext();

    return EXIT_SUCCESS;
}

}  // namespace Graphene
