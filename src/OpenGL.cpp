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
#include <cassert>

namespace Graphene {

PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBINDBUFFERBASEPROC glBindBufferBase;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLBINDTEXTUREPROC glBindTexture;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBLENDFUNCPROC glBlendFunc;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLCLEARPROC glClear;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
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
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSTRINGPROC glGetString;
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

#define glGetProcAddress(name) glXGetProcAddress(reinterpret_cast<const GLubyte*>((name)))

namespace OpenGL {

void loadCore() {
    glActiveTexture = reinterpret_cast<PFNGLACTIVETEXTUREPROC>(glGetProcAddress("glActiveTexture"));
    glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(glGetProcAddress("glAttachShader"));
    glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(glGetProcAddress("glBindBuffer"));
    glBindBufferBase = reinterpret_cast<PFNGLBINDBUFFERBASEPROC>(glGetProcAddress("glBindBufferBase"));
    glBindFramebuffer = reinterpret_cast<PFNGLBINDFRAMEBUFFERPROC>(glGetProcAddress("glBindFramebuffer"));
    glBindTexture = reinterpret_cast<PFNGLBINDTEXTUREPROC>(glGetProcAddress("glBindTexture"));
    glBindVertexArray = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>(glGetProcAddress("glBindVertexArray"));
    glBlendFunc = reinterpret_cast<PFNGLBLENDFUNCPROC>(glGetProcAddress("glBlendFunc"));
    glBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>(glGetProcAddress("glBufferData"));
    glBufferSubData = reinterpret_cast<PFNGLBUFFERSUBDATAPROC>(glGetProcAddress("glBufferSubData"));
    glClear = reinterpret_cast<PFNGLCLEARPROC>(glGetProcAddress("glClear"));
    glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>(glGetProcAddress("glCompileShader"));
    glCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>(glGetProcAddress("glCreateProgram"));
    glCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>(glGetProcAddress("glCreateShader"));
    glDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(glGetProcAddress("glDeleteBuffers"));
    glDeleteFramebuffers = reinterpret_cast<PFNGLDELETEFRAMEBUFFERSPROC>(glGetProcAddress("glDeleteFramebuffers"));
    glDeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>(glGetProcAddress("glDeleteProgram"));
    glDeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>(glGetProcAddress("glDeleteShader"));
    glDeleteTextures = reinterpret_cast<PFNGLDELETETEXTURESPROC>(glGetProcAddress("glDeleteTextures"));
    glDeleteVertexArrays = reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>(glGetProcAddress("glDeleteVertexArrays"));
    glDisable = reinterpret_cast<PFNGLDISABLEPROC>(glGetProcAddress("glDisable"));
    glDrawBuffer = reinterpret_cast<PFNGLDRAWBUFFERPROC>(glGetProcAddress("glDrawBuffer"));
    glDrawBuffers = reinterpret_cast<PFNGLDRAWBUFFERSPROC>(glGetProcAddress("glDrawBuffers"));
    glDrawElements = reinterpret_cast<PFNGLDRAWELEMENTSPROC>(glGetProcAddress("glDrawElements"));
    glEnable = reinterpret_cast<PFNGLENABLEPROC>(glGetProcAddress("glEnable"));
    glEnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(glGetProcAddress("glEnableVertexAttribArray"));
    glFramebufferTexture = reinterpret_cast<PFNGLFRAMEBUFFERTEXTUREPROC>(glGetProcAddress("glFramebufferTexture"));
    glFrontFace = reinterpret_cast<PFNGLFRONTFACEPROC>(glGetProcAddress("glFrontFace"));
    glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(glGetProcAddress("glGenBuffers"));
    glGenerateMipmap = reinterpret_cast<PFNGLGENERATEMIPMAPPROC>(glGetProcAddress("glGenerateMipmap"));
    glGenFramebuffers = reinterpret_cast<PFNGLGENFRAMEBUFFERSPROC>(glGetProcAddress("glGenFramebuffers"));
    glGenTextures = reinterpret_cast<PFNGLGENTEXTURESPROC>(glGetProcAddress("glGenTextures"));
    glGenVertexArrays = reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>(glGetProcAddress("glGenVertexArrays"));
    glGetProgramInfoLog = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(glGetProcAddress("glGetProgramInfoLog"));
    glGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>(glGetProcAddress("glGetProgramiv"));
    glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(glGetProcAddress("glGetShaderInfoLog"));
    glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>(glGetProcAddress("glGetShaderiv"));
    glGetString = reinterpret_cast<PFNGLGETSTRINGPROC>(glGetProcAddress("glGetString"));
    glGetUniformBlockIndex = reinterpret_cast<PFNGLGETUNIFORMBLOCKINDEXPROC>(glGetProcAddress("glGetUniformBlockIndex"));
    glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(glGetProcAddress("glGetUniformLocation"));
    glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(glGetProcAddress("glLinkProgram"));
    glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>(glGetProcAddress("glShaderSource"));
    glTexParameteri = reinterpret_cast<PFNGLTEXPARAMETERIPROC>(glGetProcAddress("glTexParameteri"));
    glTexStorage2D = reinterpret_cast<PFNGLTEXSTORAGE2DPROC>(glGetProcAddress("glTexStorage2D"));
    glTexSubImage2D = reinterpret_cast<PFNGLTEXSUBIMAGE2DPROC>(glGetProcAddress("glTexSubImage2D"));
    glUniform1f = reinterpret_cast<PFNGLUNIFORM1FPROC>(glGetProcAddress("glUniform1f"));
    glUniform1i = reinterpret_cast<PFNGLUNIFORM1IPROC>(glGetProcAddress("glUniform1i"));
    glUniform3fv = reinterpret_cast<PFNGLUNIFORM3FVPROC>(glGetProcAddress("glUniform3fv"));
    glUniform4fv = reinterpret_cast<PFNGLUNIFORM4FVPROC>(glGetProcAddress("glUniform4fv"));
    glUniformBlockBinding = reinterpret_cast<PFNGLUNIFORMBLOCKBINDINGPROC>(glGetProcAddress("glUniformBlockBinding"));
    glUniformMatrix3fv = reinterpret_cast<PFNGLUNIFORMMATRIX3FVPROC>(glGetProcAddress("glUniformMatrix3fv"));
    glUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>(glGetProcAddress("glUniformMatrix4fv"));
    glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(glGetProcAddress("glUseProgram"));
    glVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>(glGetProcAddress("glVertexAttribPointer"));
    glViewport = reinterpret_cast<PFNGLVIEWPORTPROC>(glGetProcAddress("glViewport"));

    assert(glActiveTexture != nullptr);
    assert(glAttachShader != nullptr);
    assert(glBindBuffer != nullptr);
    assert(glBindBufferBase != nullptr);
    assert(glBindFramebuffer != nullptr);
    assert(glBindTexture != nullptr);
    assert(glBindVertexArray != nullptr);
    assert(glBlendFunc != nullptr);
    assert(glBufferData != nullptr);
    assert(glBufferSubData != nullptr);
    assert(glClear != nullptr);
    assert(glCompileShader != nullptr);
    assert(glCreateProgram != nullptr);
    assert(glCreateShader != nullptr);
    assert(glDeleteBuffers != nullptr);
    assert(glDeleteFramebuffers != nullptr);
    assert(glDeleteProgram != nullptr);
    assert(glDeleteShader != nullptr);
    assert(glDeleteTextures != nullptr);
    assert(glDeleteVertexArrays != nullptr);
    assert(glDisable != nullptr);
    assert(glDrawBuffer != nullptr);
    assert(glDrawBuffers != nullptr);
    assert(glDrawElements != nullptr);
    assert(glEnable != nullptr);
    assert(glEnableVertexAttribArray != nullptr);
    assert(glFramebufferTexture != nullptr);
    assert(glFrontFace != nullptr);
    assert(glGenBuffers != nullptr);
    assert(glGenerateMipmap != nullptr);
    assert(glGenFramebuffers != nullptr);
    assert(glGenTextures != nullptr);
    assert(glGenVertexArrays != nullptr);
    assert(glGetProgramInfoLog != nullptr);
    assert(glGetProgramiv != nullptr);
    assert(glGetShaderInfoLog != nullptr);
    assert(glGetShaderiv != nullptr);
    assert(glGetString != nullptr);
    assert(glGetUniformBlockIndex != nullptr);
    assert(glGetUniformLocation != nullptr);
    assert(glLinkProgram != nullptr);
    assert(glShaderSource != nullptr);
    assert(glTexParameteri != nullptr);
    assert(glTexStorage2D != nullptr);
    assert(glTexSubImage2D != nullptr);
    assert(glUniform1f != nullptr);
    assert(glUniform1i != nullptr);
    assert(glUniform3fv != nullptr);
    assert(glUniform4fv != nullptr);
    assert(glUniformBlockBinding != nullptr);
    assert(glUniformMatrix3fv != nullptr);
    assert(glUniformMatrix4fv != nullptr);
    assert(glUseProgram != nullptr);
    assert(glVertexAttribPointer != nullptr);
    assert(glViewport != nullptr);
}

void loadExt() {
    glDebugMessageCallbackARB = reinterpret_cast<PFNGLDEBUGMESSAGECALLBACKARBPROC>(glGetProcAddress("glDebugMessageCallbackARB"));
    glDebugMessageControlARB = reinterpret_cast<PFNGLDEBUGMESSAGECONTROLARBPROC>(glGetProcAddress("glDebugMessageControlARB"));

    assert(glDebugMessageCallbackARB != nullptr);
    assert(glDebugMessageControlARB != nullptr);
}

}  // namespace OpenGL

}  // namespace Graphene
