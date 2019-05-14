#include <Window.h>
#include <OpenGL.h>
#include <windowsx.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

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

#define SetWindowObject(window, object) SetWindowLongPtr((window), GWLP_USERDATA, reinterpret_cast<LONG_PTR>((object)))
#define GetWindowObject(window) reinterpret_cast<Window*>(GetWindowLongPtr((window), GWLP_USERDATA))

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    Window* self = GetWindowObject(window);

    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_MOUSEMOVE: {
            int xPosition = GET_X_LPARAM(lParam);
            int yPosition = GET_Y_LPARAM(lParam);

            if (self->isMouseCaptured()) {
                POINT windowCenter = { self->getWidth() >> 1, self->getHeight() >> 1 };
                xPosition -= windowCenter.x;
                yPosition -= windowCenter.y;

                if (xPosition != 0 || yPosition != 0) {
                    ClientToScreen(window, &windowCenter);
                    SetCursorPos(windowCenter.x, windowCenter.y);
                }
            }

            return 0;
        }
    }

    return DefWindowProc(window, message, wParam, lParam);
}

Window::Window(int width, int height):
        width(width),
        height(height) {
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);

    this->instance = GetModuleHandle(nullptr);
    this->window = this->createWindow(L"OpenGL Class", L"OpenGL Window", WindowProc);

    SetWindowObject(this->window, this);
    ShowWindow(this->window, SW_SHOWDEFAULT);
    UpdateWindow(this->window);
}

Window::~Window() {
    this->destroyWindow(this->window);

    FreeConsole();
}

int Window::getWidth() const {
    return this->width;
}

int Window::getHeight() const {
    return this->height;
}

void Window::createContext() {
    HWND dummyWindow = this->createWindow(L"Dummy Class", L"Dummy Window", DefWindowProc);
    this->createBaseContext(dummyWindow);

    OpenGL::loadWglExt();

    this->destroyContext();
    this->destroyWindow(dummyWindow);

    this->createExtContext(this->window);

    OpenGL::loadCore();
    OpenGL::loadExt();

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
    glDebugMessageCallbackARB(debugHandler, nullptr);

    std::cout
        << "Vendor: " << glGetString(GL_VENDOR) << std::endl
        << "Renderer: " << glGetString(GL_RENDERER) << std::endl
        << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl
        << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glFrontFace(GL_CW);
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_CULL_FACE);
}

void Window::destroyContext() {
    if (this->deviceContext != nullptr) {
        wglMakeCurrent(this->deviceContext, nullptr);
    }

    if (this->renderingContext != nullptr) {
        wglDeleteContext(this->renderingContext);
        this->renderingContext = nullptr;
    }

    if (this->deviceContext != nullptr) {
        ReleaseDC(this->window, this->deviceContext);
        this->deviceContext = nullptr;
    }
}

std::vector<bool> Window::getKeyboardState() const {
    return this->getVirtualKeysState(VK_BACK, VK_OEM_CLEAR);
}

std::vector<bool> Window::getMouseState() const {
    return this->getVirtualKeysState(VK_LBUTTON, VK_XBUTTON2);
}

std::pair<int, int> Window::getMousePosition() const {
    POINT cursorPosition;
    GetCursorPos(&cursorPosition);
    ScreenToClient(this->window, &cursorPosition);

    return std::make_pair(cursorPosition.x, cursorPosition.y);
}

bool Window::isMouseCaptured() const {
    return this->mouseCaptured;
}

void Window::captureMouse(bool capture) {
    this->mouseCaptured = capture;

    if (this->isMouseCaptured()) {
        POINT windowCenter = { this->getWidth() >> 1, this->getHeight() >> 1 };
        ClientToScreen(this->window, &windowCenter);
        SetCursorPos(windowCenter.x, windowCenter.y);
    }

    ShowCursor(this->isMouseCaptured() ? FALSE : TRUE);
}

void Window::update() {
    SwapBuffers(this->deviceContext);
}

bool Window::dispatchEvents() {
    MSG message;
    bool keepRunning = true;

    while (keepRunning && PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);

        keepRunning = (message.message != WM_QUIT);
    }

    return keepRunning;
}

HWND Window::createWindow(LPCWSTR className, LPCWSTR windowName, WNDPROC windowProc) {
    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = windowProc;
    wcex.hInstance = this->instance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = HBRUSH(COLOR_WINDOW);
    wcex.lpszClassName = className;

    if (!RegisterClassEx(&wcex)) {
        throw std::runtime_error("RegisterClassEx()");
    }

    HWND window = CreateWindow(
        className, windowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, this->getWidth(), this->getHeight(),
        nullptr, nullptr, this->instance, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("CreateWindow()");
    }

    return window;
}

void Window::destroyWindow(HWND window) {
    WCHAR className[64];

    if (window != nullptr) {
        GetClassName(window, className, 64);
        DestroyWindow(window);
    }

    UnregisterClass(className, this->instance);
}

void Window::createBaseContext(HWND window) {
    this->deviceContext = GetDC(window);
    if (this->deviceContext == nullptr) {
        throw std::runtime_error("GetDC()");
    }

    PIXELFORMATDESCRIPTOR pfd = { };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixelFormat = ChoosePixelFormat(this->deviceContext, &pfd);
    if (pixelFormat == 0) {
        throw std::runtime_error("ChoosePixelFormat()");
    }

    if (!SetPixelFormat(this->deviceContext, pixelFormat, &pfd)) {
        throw std::runtime_error("SetPixelFormat()");
    }

    this->renderingContext = wglCreateContext(this->deviceContext);
    if (this->renderingContext == nullptr) {
        throw std::runtime_error("wglCreateContext()");
    }

    if (!wglMakeCurrent(this->deviceContext, this->renderingContext)) {
        throw std::runtime_error("wglMakeCurrent()");
    }
}

void Window::createExtContext(HWND window) {
    this->deviceContext = GetDC(window);
    if (this->deviceContext == nullptr) {
        throw std::runtime_error("GetDC()");
    }

    const int pixelAttribList[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0
    };

    int pixelFormat;
    UINT numberFormats;
    if (!wglChoosePixelFormatARB(this->deviceContext, pixelAttribList, nullptr, 1, &pixelFormat, &numberFormats)) {
        throw std::runtime_error("wglChoosePixelFormatARB()");
    }

    PIXELFORMATDESCRIPTOR pfd;
    if (!SetPixelFormat(this->deviceContext, pixelFormat, &pfd)) {
        throw std::runtime_error("SetPixelFormat()");
    }

    const int contextAttribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    };

    this->renderingContext = wglCreateContextAttribsARB(this->deviceContext, nullptr, contextAttribList);
    if (this->renderingContext == nullptr) {
        throw std::runtime_error("wglCreateContextAttribsARB()");
    }

    if (!wglMakeCurrent(this->deviceContext, this->renderingContext)) {
        throw std::runtime_error("wglMakeCurrent()");
    }
}

std::vector<bool> Window::getVirtualKeysState(int startKey, int endKey) const {
    std::vector<bool> virtualKeysState(endKey);
    int virtualKey = startKey;

    std::generate(virtualKeysState.begin(), virtualKeysState.end(), [&virtualKey]() {
        return GetAsyncKeyState(virtualKey++) & (1 << 15);  // Check MSB
    });

    return virtualKeysState;
}

}  // namespace Graphene
