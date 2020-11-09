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
#include <unordered_set>

#if defined(_WIN32)
#include <windows.h>
#include <GL/wglext.h>
#elif defined(__linux__)
#include <GL/glx.h>
#include <GL/glxext.h>
#endif

namespace Graphene {

extern GRAPHENE_API PFNGLACTIVETEXTUREPROC glActiveTexture;
extern GRAPHENE_API PFNGLATTACHSHADERPROC glAttachShader;
extern GRAPHENE_API PFNGLBINDBUFFERPROC glBindBuffer;
extern GRAPHENE_API PFNGLBINDBUFFERBASEPROC glBindBufferBase;
extern GRAPHENE_API PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern GRAPHENE_API PFNGLBINDTEXTUREPROC glBindTexture;
extern GRAPHENE_API PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern GRAPHENE_API PFNGLBLENDCOLORPROC glClearColor;
extern GRAPHENE_API PFNGLBLENDFUNCPROC glBlendFunc;
extern GRAPHENE_API PFNGLBUFFERDATAPROC glBufferData;
extern GRAPHENE_API PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern GRAPHENE_API PFNGLCLEARPROC glClear;
extern GRAPHENE_API PFNGLCOMPILESHADERPROC glCompileShader;
extern GRAPHENE_API PFNGLCREATEPROGRAMPROC glCreateProgram;
extern GRAPHENE_API PFNGLCREATESHADERPROC glCreateShader;
extern GRAPHENE_API PFNGLCULLFACEPROC glCullFace;
extern GRAPHENE_API PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern GRAPHENE_API PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern GRAPHENE_API PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern GRAPHENE_API PFNGLDELETESHADERPROC glDeleteShader;
extern GRAPHENE_API PFNGLDELETETEXTURESPROC glDeleteTextures;
extern GRAPHENE_API PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern GRAPHENE_API PFNGLDEPTHFUNCPROC glDepthFunc;
extern GRAPHENE_API PFNGLDISABLEPROC glDisable;
extern GRAPHENE_API PFNGLDRAWBUFFERPROC glDrawBuffer;
extern GRAPHENE_API PFNGLDRAWBUFFERSPROC glDrawBuffers;
extern GRAPHENE_API PFNGLDRAWELEMENTSPROC glDrawElements;
extern GRAPHENE_API PFNGLENABLEPROC glEnable;
extern GRAPHENE_API PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern GRAPHENE_API PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
extern GRAPHENE_API PFNGLFRONTFACEPROC glFrontFace;
extern GRAPHENE_API PFNGLGENBUFFERSPROC glGenBuffers;
extern GRAPHENE_API PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern GRAPHENE_API PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern GRAPHENE_API PFNGLGENTEXTURESPROC glGenTextures;
extern GRAPHENE_API PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern GRAPHENE_API PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName;
extern GRAPHENE_API PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
extern GRAPHENE_API PFNGLGETERRORPROC glGetError;
extern GRAPHENE_API PFNGLGETINTEGERVPROC glGetIntegerv;
extern GRAPHENE_API PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern GRAPHENE_API PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern GRAPHENE_API PFNGLREADPIXELSPROC glReadPixels;
extern GRAPHENE_API PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern GRAPHENE_API PFNGLGETSHADERIVPROC glGetShaderiv;
extern GRAPHENE_API PFNGLGETSTRINGPROC glGetString;
extern GRAPHENE_API PFNGLGETSTRINGIPROC glGetStringi;
extern GRAPHENE_API PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
extern GRAPHENE_API PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern GRAPHENE_API PFNGLLINEWIDTHPROC glLineWidth;
extern GRAPHENE_API PFNGLLINKPROGRAMPROC glLinkProgram;
extern GRAPHENE_API PFNGLSHADERSOURCEPROC glShaderSource;
extern GRAPHENE_API PFNGLTEXPARAMETERIPROC glTexParameteri;
extern GRAPHENE_API PFNGLTEXSTORAGE2DPROC glTexStorage2D;
extern GRAPHENE_API PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;
extern GRAPHENE_API PFNGLUNIFORM1FPROC glUniform1f;
extern GRAPHENE_API PFNGLUNIFORM1IPROC glUniform1i;
extern GRAPHENE_API PFNGLUNIFORM3FVPROC glUniform3fv;
extern GRAPHENE_API PFNGLUNIFORM4FVPROC glUniform4fv;
extern GRAPHENE_API PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
extern GRAPHENE_API PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern GRAPHENE_API PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern GRAPHENE_API PFNGLUSEPROGRAMPROC glUseProgram;
extern GRAPHENE_API PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern GRAPHENE_API PFNGLVIEWPORTPROC glViewport;

extern GRAPHENE_API PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB;  // GL_ARB_debug_output
extern GRAPHENE_API PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;  // GL_ARB_debug_output

#if defined(_WIN32)
extern GRAPHENE_API PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;  // WGL_ARB_pixel_format
extern GRAPHENE_API PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;  // WGL_ARB_create_context
extern GRAPHENE_API PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;  // WGL_ARB_extensions_string
extern GRAPHENE_API PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;  // WGL_EXT_swap_control, WGL_EXT_swap_control_tear
#elif defined(__linux__)
extern GRAPHENE_API PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;  // GLX_ARB_create_context
extern GRAPHENE_API PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;  // GLX_EXT_swap_control, GLX_EXT_swap_control_tear
extern GRAPHENE_API PFNGLXSWAPINTERVALMESAPROC glXSwapIntervalMESA;  // GLX_MESA_swap_control
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
GRAPHENE_API const std::unordered_set<std::string>& getSupportedExtensions();

}  // namespace OpenGL

}  // namespace Graphene

#endif  // OPENGL_H
