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

#if defined(_WIN32)

#include <WindowWin32.h>
#include <OpenGL.h>
#include <windowsx.h>
#include <stdexcept>
#include <algorithm>
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

#define SetWindowObject(window, object) SetWindowLongPtr((window), GWLP_USERDATA, reinterpret_cast<LONG_PTR>((object)))
#define GetWindowObject(window) reinterpret_cast<WindowWin32*>(GetWindowLongPtr((window), GWLP_USERDATA))

LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    WindowWin32* self = GetWindowObject(window);

    switch (message) {
        case WM_KEYDOWN:
        case WM_KEYUP: {
            int scancode = HIWORD(lParam) & 0xFF;  // lParam 16-23 bits is the keyboard scancode
            bool keyState = (message == WM_KEYDOWN);
            self->setKeyboardState(scancode, keyState);
            self->onKeyboardButton(scancode, keyState);
            return 0;
        }

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP: {
            bool mouseButtonState = (message == WM_LBUTTONDOWN);
            self->setMouseState(MouseButton::LEFT, mouseButtonState);
            self->onMouseButton(MouseButton::LEFT, mouseButtonState);
            return 0;
        }

        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP: {
            bool mouseButtonState = (message == WM_MBUTTONDOWN);
            self->setMouseState(MouseButton::MIDDLE, mouseButtonState);
            self->onMouseButton(MouseButton::MIDDLE, mouseButtonState);
            return 0;
        }

        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP: {
            bool mouseButtonState = (message == WM_RBUTTONDOWN);
            self->setMouseState(MouseButton::RIGHT, mouseButtonState);
            self->onMouseButton(MouseButton::RIGHT, mouseButtonState);
            return 0;
        }

        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP: {
            MouseButton mouseButton = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? MouseButton::X1 : MouseButton::X2;
            bool mouseButtonState = (message == WM_XBUTTONDOWN);
            self->setMouseState(mouseButton, mouseButtonState);
            self->onMouseButton(mouseButton, mouseButtonState);
            return 0;
        }

        case WM_MOUSEMOVE: {
            int xPosition = GET_X_LPARAM(lParam);
            int yPosition = GET_Y_LPARAM(lParam);
            bool mouseMoved = true;

            if (self->isMouseCaptured()) {
                POINT windowCenter = { self->width >> 1, self->height >> 1 };
                xPosition -= windowCenter.x;
                yPosition -= windowCenter.y;

                if (xPosition != 0 || yPosition != 0) {
                    self->warpMousePointer(windowCenter.x, windowCenter.y);
                } else {
                    mouseMoved = false; // WM_MOUSEMOVE after SetCursorPos() to window center
                }
            }

            if (mouseMoved) {
                self->setMousePosition(xPosition, yPosition);
                self->onMouseMotion(xPosition, yPosition);
            }

            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(window, message, wParam, lParam);
}

WindowWin32::WindowWin32(int width, int height):
        RenderTarget(width, height) {
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);

    this->instance = GetModuleHandle(nullptr);
    this->window = this->createWindow(L"OpenGL Class", L"OpenGL Window", WindowProc);
    this->createContext();

    SetWindowObject(this->window, this);
    ShowWindow(this->window, SW_SHOWDEFAULT);
    UpdateWindow(this->window);

    this->setMousePosition(this->width >> 1, this->height >> 1);
}

WindowWin32::~WindowWin32() {
    this->destroyContext();
    this->destroyWindow(this->window);

    FreeConsole();
}

const KeyboardState& WindowWin32::getKeyboardState() const {
    return this->keyboardState;
}

const MouseState& WindowWin32::getMouseState() const {
    return this->mouseState;
}

const MousePosition& WindowWin32::getMousePosition() const {
    return this->mousePosition;
}

bool WindowWin32::isMouseCaptured() const {
    return this->mouseCaptured;
}

void WindowWin32::captureMouse(bool capture) {
    this->mouseCaptured = capture;

    if (this->mouseCaptured) {
        POINT windowCenter = { this->width >> 1, this->height >> 1 };
        this->warpMousePointer(windowCenter.x, windowCenter.y);
        this->setMousePosition(windowCenter.x, windowCenter.y);
    }

    ShowCursor(this->mouseCaptured ? FALSE : TRUE);
}

void WindowWin32::update() {
    RenderTarget::update();
    SwapBuffers(this->deviceContext);
}

void WindowWin32::dispatchEvents() {
    MSG message;

    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        DispatchMessage(&message);

        if (message.message == WM_QUIT) {
            this->onQuit();
            break;
        }
    }

    this->onIdle();
}

void WindowWin32::setKeyboardState(int scancode, bool state) {
    this->keyboardState[scancode] = state;
}

void WindowWin32::setMouseState(MouseButton button, bool state) {
    this->mouseState[button] = state;
}

void WindowWin32::setMousePosition(int x, int y) {
    this->mousePosition.first = x;
    this->mousePosition.second = y;
}

void WindowWin32::warpMousePointer(int x, int y) {
    POINT position = { x, y };
    ClientToScreen(this->window, &position);
    SetCursorPos(position.x, position.y);
}

HWND WindowWin32::createWindow(LPCWSTR className, LPCWSTR windowName, WNDPROC windowProc) {
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
        CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->height,
        nullptr, nullptr, this->instance, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("CreateWindow()");
    }

    return window;
}

void WindowWin32::destroyWindow(HWND window) {
    WCHAR className[64];

    if (window != nullptr) {
        GetClassName(window, className, 64);
        DestroyWindow(window);
    }

    UnregisterClass(className, this->instance);
}

void WindowWin32::createContext() {
    HWND dummyWindow = this->createWindow(L"Dummy Class", L"Dummy Window", DefWindowProc);
    this->createBaseContext(dummyWindow);

    OpenGL::loadWglExtensions();

    this->destroyContext();
    this->destroyWindow(dummyWindow);

    this->createExtContext(this->window);

    OpenGL::loadCore();
    OpenGL::loadExtensions();

    if (OpenGL::isExtensionSupported("GL_ARB_debug_output")) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
        glDebugMessageCallbackARB(debugHandler, nullptr);
    }
    else {
        std::cout << "GL_ARB_debug_output unavailable, OpenGL debug disabled" << std::endl;
    }

    std::cout
        << "Vendor: " << glGetString(GL_VENDOR) << std::endl
        << "Renderer: " << glGetString(GL_RENDERER) << std::endl
        << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl
        << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glFrontFace(GL_CW);
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_CULL_FACE);
}

void WindowWin32::createBaseContext(HWND window) {
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

void WindowWin32::createExtContext(HWND window) {
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

void WindowWin32::destroyContext() {
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

}  // namespace Graphene

#endif  // defined(_WIN32)
