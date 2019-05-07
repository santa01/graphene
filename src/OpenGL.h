#pragma once

#define GL_GLEXT_PROTOTYPES
#include <GL/glcorearb.h>
#include <GL/wglext.h>

namespace OpenGL
{
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB; // WGL_ARB_pixel_format
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB; // WGL_ARB_create_context

extern PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB; // ARB_debug_output
extern PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB; // ARB_debug_output

void loadExtensions();
}
