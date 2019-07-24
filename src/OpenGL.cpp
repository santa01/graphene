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

#include <OpenGL.h>
#include <Logger.h>
#include <stdexcept>
#include <unordered_set>

namespace Graphene {

PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBINDBUFFERBASEPROC glBindBufferBase;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLBINDTEXTUREPROC glBindTexture;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBLENDCOLORPROC glClearColor;
PFNGLBLENDFUNCPROC glBlendFunc;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLCLEARPROC glClear;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLCULLFACEPROC glCullFace;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDELETETEXTURESPROC glDeleteTextures;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDISABLEPROC glDisable;
PFNGLDRAWBUFFERPROC glDrawBuffer;
PFNGLDRAWBUFFERSPROC glDrawBuffers;
PFNGLDRAWELEMENTSPROC glDrawElements;
PFNGLENABLEPROC glEnable;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
PFNGLFRONTFACEPROC glFrontFace;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLGENTEXTURESPROC glGenTextures;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGETINTEGERVPROC glGetIntegerv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSTRINGPROC glGetString;
PFNGLGETSTRINGIPROC glGetStringi;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLTEXPARAMETERIPROC glTexParameteri;
PFNGLTEXSTORAGE2DPROC glTexStorage2D;
PFNGLTEXSUBIMAGE2DPROC glTexSubImage2D;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLVIEWPORTPROC glViewport;

PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;
PFNGLDEBUGMESSAGECONTROLARBPROC glDebugMessageControlARB;

#if defined(_WIN32)
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
#elif defined(__linux__)
PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB;
PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;
#endif

#if defined(_WIN32)
PROC glGetProcAddress(LPCSTR name) {
    PROC procAddress = wglGetProcAddress(name);
    if (procAddress == nullptr) {
        // OpenGL 1.1 functions are exported from opengl32.dll
        HMODULE opengl32 = GetModuleHandle(L"opengl32.dll");
        procAddress = GetProcAddress(opengl32, name);
    }

    return procAddress;
}
#elif defined(__linux__)
#define glGetProcAddress(name) glXGetProcAddress(reinterpret_cast<const GLubyte*>((name)))
#endif

#define LOAD_PROC_ADDR(proc)                                            \
do {                                                                    \
    proc = reinterpret_cast<decltype(proc)>(glGetProcAddress(#proc));   \
} while (0)

#define LOAD_MANDATORY(proc)                                            \
do {                                                                    \
    LOAD_PROC_ADDR(proc);                                               \
    if (proc == nullptr) {                                              \
        throw std::runtime_error(LogFormat("%s == nullptr", #proc));    \
    }                                                                   \
} while(0)

#define LOAD_OPTIONAL LOAD_PROC_ADDR

namespace OpenGL {

void loadCore() {
    LOAD_MANDATORY(glActiveTexture);
    LOAD_MANDATORY(glAttachShader);
    LOAD_MANDATORY(glBindBuffer);
    LOAD_MANDATORY(glBindBufferBase);
    LOAD_MANDATORY(glBindFramebuffer);
    LOAD_MANDATORY(glBindTexture);
    LOAD_MANDATORY(glBindVertexArray);
    LOAD_MANDATORY(glClearColor);
    LOAD_MANDATORY(glBlendFunc);
    LOAD_MANDATORY(glBufferData);
    LOAD_MANDATORY(glBufferSubData);
    LOAD_MANDATORY(glClear);
    LOAD_MANDATORY(glCompileShader);
    LOAD_MANDATORY(glCreateProgram);
    LOAD_MANDATORY(glCreateShader);
    LOAD_MANDATORY(glCullFace);
    LOAD_MANDATORY(glDeleteBuffers);
    LOAD_MANDATORY(glDeleteFramebuffers);
    LOAD_MANDATORY(glDeleteProgram);
    LOAD_MANDATORY(glDeleteShader);
    LOAD_MANDATORY(glDeleteTextures);
    LOAD_MANDATORY(glDeleteVertexArrays);
    LOAD_MANDATORY(glDisable);
    LOAD_MANDATORY(glDrawBuffer);
    LOAD_MANDATORY(glDrawBuffers);
    LOAD_MANDATORY(glDrawElements);
    LOAD_MANDATORY(glEnable);
    LOAD_MANDATORY(glEnableVertexAttribArray);
    LOAD_MANDATORY(glFramebufferTexture);
    LOAD_MANDATORY(glFrontFace);
    LOAD_MANDATORY(glGenBuffers);
    LOAD_MANDATORY(glGenerateMipmap);
    LOAD_MANDATORY(glGenFramebuffers);
    LOAD_MANDATORY(glGenTextures);
    LOAD_MANDATORY(glGenVertexArrays);
    LOAD_MANDATORY(glGetIntegerv);
    LOAD_MANDATORY(glGetProgramInfoLog);
    LOAD_MANDATORY(glGetProgramiv);
    LOAD_MANDATORY(glGetShaderInfoLog);
    LOAD_MANDATORY(glGetShaderiv);
    LOAD_MANDATORY(glGetString);
    LOAD_MANDATORY(glGetStringi);
    LOAD_MANDATORY(glGetUniformBlockIndex);
    LOAD_MANDATORY(glGetUniformLocation);
    LOAD_MANDATORY(glLinkProgram);
    LOAD_MANDATORY(glShaderSource);
    LOAD_MANDATORY(glTexParameteri);
    LOAD_MANDATORY(glTexStorage2D);
    LOAD_MANDATORY(glTexSubImage2D);
    LOAD_MANDATORY(glUniform1f);
    LOAD_MANDATORY(glUniform1i);
    LOAD_MANDATORY(glUniform3fv);
    LOAD_MANDATORY(glUniform4fv);
    LOAD_MANDATORY(glUniformBlockBinding);
    LOAD_MANDATORY(glUniformMatrix3fv);
    LOAD_MANDATORY(glUniformMatrix4fv);
    LOAD_MANDATORY(glUseProgram);
    LOAD_MANDATORY(glVertexAttribPointer);
    LOAD_MANDATORY(glViewport);
}

void loadExtensions() {
    LOAD_OPTIONAL(glDebugMessageControlARB);
    LOAD_OPTIONAL(glDebugMessageCallbackARB);
}

#if defined(_WIN32)
void loadWglExtensions() {
    LOAD_MANDATORY(wglChoosePixelFormatARB);
    LOAD_MANDATORY(wglCreateContextAttribsARB);
    LOAD_MANDATORY(wglGetExtensionsStringARB);
    LOAD_OPTIONAL(wglSwapIntervalEXT);
}
#elif defined(__linux__)
void loadGlxExtensions() {
    LOAD_MANDATORY(glXCreateContextAttribsARB);
    LOAD_OPTIONAL(glXSwapIntervalEXT);
}
#endif

std::unordered_set<std::string> availableExtensions;

bool isExtensionSupported(const std::string& extension) {
    if (availableExtensions.empty()) {
        int numExtensions;
        glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

        for (int i = 0; i < numExtensions; i++) {
            availableExtensions.insert(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
        }
    }

    return availableExtensions.find(extension) != availableExtensions.end();
}

}  // namespace OpenGL

}  // namespace Graphene
