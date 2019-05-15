#include <OpenGL.h>
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

PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

namespace OpenGL {

std::unordered_set<std::string> availableExtensions;

PROC glGetProcAddress(LPCSTR name) {
    PROC procAddress = wglGetProcAddress(name);
    if (procAddress == nullptr) {
        // OpenGL 1.1 functions are exported from opengl32.dll
        HMODULE opengl32 = GetModuleHandle(L"opengl32.dll");
        procAddress = GetProcAddress(opengl32, name);
    }
    return procAddress;
}

#define LOAD_PROC_ADDR(proc)                                            \
do {                                                                    \
    proc = reinterpret_cast<decltype(proc)>(glGetProcAddress(#proc));   \
} while (0)

#define LOAD_MANDATORY(proc)                                \
do {                                                        \
    LOAD_PROC_ADDR(proc);                                   \
    if (proc == nullptr) {                                  \
        throw std::runtime_error(#proc ## " == nullptr");   \
    }                                                       \
} while(0)

#define LOAD_OPTIONAL LOAD_PROC_ADDR

void loadCore() {
    LOAD_MANDATORY(glActiveTexture);
    LOAD_MANDATORY(glAttachShader);
    LOAD_MANDATORY(glBindBuffer);
    LOAD_MANDATORY(glBindBufferBase);
    LOAD_MANDATORY(glBindFramebuffer);
    LOAD_MANDATORY(glBindTexture);
    LOAD_MANDATORY(glBindVertexArray);
    LOAD_MANDATORY(glBlendFunc);
    LOAD_MANDATORY(glBufferData);
    LOAD_MANDATORY(glBufferSubData);
    LOAD_MANDATORY(glClear);
    LOAD_MANDATORY(glCompileShader);
    LOAD_MANDATORY(glCreateProgram);
    LOAD_MANDATORY(glCreateShader);
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
    LOAD_OPTIONAL(glDebugMessageCallbackARB);
    LOAD_OPTIONAL(glDebugMessageControlARB);
}

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

void loadWglExtensions() {
    LOAD_MANDATORY(wglChoosePixelFormatARB);
    LOAD_MANDATORY(wglCreateContextAttribsARB);
}

}  // namespace OpenGL

}  // namespace Graphene
