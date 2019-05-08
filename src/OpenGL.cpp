#include <OpenGL.h>

namespace Graphene {

namespace OpenGL {

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB = nullptr;
PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB = nullptr;

void loadExtensions() {
    wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
    wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));

    glDebugMessageControlARB = reinterpret_cast<PFNGLDEBUGMESSAGECONTROLARBPROC>(wglGetProcAddress("glDebugMessageControlARB"));
    glDebugMessageCallbackARB = reinterpret_cast<PFNGLDEBUGMESSAGECALLBACKARBPROC>(wglGetProcAddress("glDebugMessageCallbackARB"));
}

}  // namespace OpenGL

}  // namespace Graphene
