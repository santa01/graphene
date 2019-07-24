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

#ifndef OPENGL_H
#define OPENGL_H

#include <GrapheneApi.h>

// Override vendor provided gl.h
#include <GL/gl.h>  // Renamed glcorearb.h
#include <string>

#if defined(_WIN32)
#include <windows.h>
#include <GL/wglext.h>
#elif defined(__linux__)
#include <GL/glx.h>
#include <GL/glxext.h>
#endif

namespace Graphene {

extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLBINDTEXTUREPROC glBindTexture;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBLENDCOLORPROC glClearColor;
extern PFNGLBLENDFUNCPROC glBlendFunc;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLCLEARPROC glClear;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLCULLFACEPROC glCullFace;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDELETETEXTURESPROC glDeleteTextures;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLDISABLEPROC glDisable;
extern PFNGLDRAWBUFFERPROC glDrawBuffer;
extern PFNGLDRAWBUFFERSPROC glDrawBuffers;
extern PFNGLDRAWELEMENTSPROC glDrawElements;
extern PFNGLENABLEPROC glEnable;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
extern PFNGLFRONTFACEPROC glFrontFace;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLGENTEXTURESPROC glGenTextures;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLGETINTEGERVPROC glGetIntegerv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSTRINGPROC glGetString;
extern PFNGLGETSTRINGIPROC glGetStringi;
extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLTEXPARAMETERIPROC glTexParameteri;
extern PFNGLTEXSTORAGE2DPROC glTexStorage2D;
extern PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLVIEWPORTPROC glViewport;

extern PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB;  // ARB_debug_output
extern PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;  // ARB_debug_output

#if defined(_WIN32)
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;  // WGL_ARB_pixel_format
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;  // WGL_ARB_create_context
extern PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;  // WGL_ARB_extensions_string
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;  // WGL_EXT_swap_control, WGL_EXT_swap_control_tear
#elif defined(__linux__)
extern PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;  // GLX_ARB_create_context
extern PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;  // GLX_EXT_swap_control, GLX_EXT_swap_control_tear
#endif

namespace OpenGL {

GRAPHENE_API void loadCore();
GRAPHENE_API void loadExtensions();

#if defined(_WIN32)
GRAPHENE_API void loadWglExtensions();
#elif defined(__linux__)
GRAPHENE_API void loadGlxExtensions();
#endif

GRAPHENE_API bool isExtensionSupported(const std::string& extension);

}  // namespace OpenGL

}  // namespace Graphene

#endif  // OPENGL_H
