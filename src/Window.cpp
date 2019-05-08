#include <Window.h>
#include <OpenGL.h>
#include <stdexcept>

namespace Graphene {

LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(window, message, wParam, lParam);
}

Window::Window(int width, int height, HINSTANCE instance):
        width(width),
        height(height),
        instance(instance) {
    createConsole();
}

Window::~Window() {
    destroyConsole();
}

void Window::createRenderingContext() {
    createWindow();

    PIXELFORMATDESCRIPTOR pfd = {};
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

void Window::createRenderingContextARB() {
    createWindow();

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
    PIXELFORMATDESCRIPTOR pfd;

    if (!OpenGL::wglChoosePixelFormatARB(this->deviceContext, pixelAttribList, nullptr, 1, &pixelFormat, &numberFormats)) {
        throw std::runtime_error("wglChoosePixelFormatARB()");
    }

    if (!SetPixelFormat(this->deviceContext, pixelFormat, &pfd)) {
        throw std::runtime_error("SetPixelFormat()");
    }

    const int contextAttribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 0,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    this->renderingContext = OpenGL::wglCreateContextAttribsARB(this->deviceContext, nullptr, contextAttribList);
    if (this->renderingContext == nullptr) {
        throw std::runtime_error("wglCreateContextAttribsARB()");
    }

    if (!wglMakeCurrent(this->deviceContext, this->renderingContext)) {
        throw std::runtime_error("wglMakeCurrent()");
    }
}

void Window::destroyRenderingContext() {
    if (this->deviceContext != nullptr) {
        wglMakeCurrent(this->deviceContext, nullptr);
    }

    if (this->renderingContext != nullptr) {
        wglDeleteContext(this->renderingContext);
        this->renderingContext = nullptr;
    }

    destroyWindow();
}

void Window::show() {
    ShowWindow(this->window, SW_SHOWDEFAULT);
    UpdateWindow(this->window);
}

void Window::processEvents(bool oneShot) {
    MSG message;
    bool breakOrbit = false;

    while (!breakOrbit && GetMessage(&message, this->window, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessage(&message);

        breakOrbit = oneShot;
    }
}

void Window::createConsole() {
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
}

void Window::destroyConsole() {
    FreeConsole();
}

void Window::createWindow() {
    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = windowProc;
    wcex.hInstance = this->instance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = HBRUSH(COLOR_WINDOW);
    wcex.lpszClassName = this->windowClassName;

    if (!RegisterClassEx(&wcex)) {
        throw std::runtime_error("RegisterClassEx()");
    }

    this->window = CreateWindow(
        this->windowClassName, this->windowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->height, nullptr, nullptr, this->instance, nullptr);
    if (this->window == nullptr) {
        throw std::runtime_error("CreateWindow()");
    }

    this->deviceContext = GetDC(this->window);
    if (this->deviceContext == nullptr) {
        throw std::runtime_error("GetDC()");
    }
}

void Window::destroyWindow() {
    if (this->window != nullptr) {
        if (this->deviceContext != nullptr) {
            ReleaseDC(this->window, this->deviceContext);
            this->deviceContext = nullptr;
        }

        DestroyWindow(this->window);
        this->window = nullptr;
    }

    UnregisterClass(this->windowClassName, this->instance);
}

}  // namespace Graphene
