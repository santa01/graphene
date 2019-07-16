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

#include <Win32Window.h>
#include <Logger.h>
#include <RenderTarget.h>
#include <windowsx.h>
#include <stdexcept>

namespace Graphene {

#define SetWindowObject(window, object) SetWindowLongPtr((window), GWLP_USERDATA, reinterpret_cast<LONG_PTR>((object)))
#define GetWindowObject(window) reinterpret_cast<Win32Window*>(GetWindowLongPtr((window), GWLP_USERDATA))

LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    Win32Window* self = GetWindowObject(window);

    switch (message) {
        case WM_KEYDOWN:
        case WM_KEYUP: {
            int scancode = HIWORD(lParam) & 0xFF;  // lParam 16-23 bits is the keyboard scancode
            bool keyState = (message == WM_KEYDOWN);
            self->keyboardState[scancode] = keyState;
            self->onKeyboardButtonSignal(scancode, keyState);
            return 0;
        }

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP: {
            bool mouseButtonState = (message == WM_LBUTTONDOWN);
            self->mouseState[MouseButton::LEFT] = mouseButtonState;
            self->onMouseButtonSignal(MouseButton::LEFT, mouseButtonState);
            return 0;
        }

        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP: {
            bool mouseButtonState = (message == WM_MBUTTONDOWN);
            self->mouseState[MouseButton::MIDDLE] = mouseButtonState;
            self->onMouseButtonSignal(MouseButton::MIDDLE, mouseButtonState);
            return 0;
        }

        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP: {
            bool mouseButtonState = (message == WM_RBUTTONDOWN);
            self->mouseState[MouseButton::RIGHT] = mouseButtonState;
            self->onMouseButtonSignal(MouseButton::RIGHT, mouseButtonState);
            return 0;
        }

        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP: {
            MouseButton mouseButton = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? MouseButton::X1 : MouseButton::X2;
            bool mouseButtonState = (message == WM_XBUTTONDOWN);
            self->mouseState[mouseButton] = mouseButtonState;
            self->onMouseButtonSignal(mouseButton, mouseButtonState);
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
                self->mousePosition = std::make_pair(xPosition, yPosition);
                self->onMouseMotionSignal(xPosition, yPosition);
            }

            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(window, message, wParam, lParam);
}

Win32Window::Win32Window(int width, int height):
        Window(width, height) {
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);

    this->instance = GetModuleHandle(nullptr);
    this->window = this->createWindow(L"OpenGL Class", L"OpenGL Window", windowProc);
    this->createContext();

    SetWindowObject(this->window, this);
    ShowWindow(this->window, SW_SHOWDEFAULT);
    UpdateWindow(this->window);
}

Win32Window::~Win32Window() {
    this->destroyContext();
    this->destroyWindow(this->window);

    FreeConsole();
}

void Win32Window::captureMouse(bool captured) {
    this->mouseCaptured = captured;

    if (this->mouseCaptured) {
        POINT windowCenter = { this->width >> 1, this->height >> 1 };
        this->mousePosition = std::make_pair(windowCenter.x, windowCenter.y);
        this->warpMousePointer(windowCenter.x, windowCenter.y);
    }

    ShowCursor(this->mouseCaptured ? FALSE : TRUE);
}

void Win32Window::update() {
    RenderTarget::update();
    SwapBuffers(this->deviceContext);
}

bool Win32Window::dispatchEvents() {
    MSG message;
    bool breakOrbit = false;

    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
        DispatchMessage(&message);

        if (message.message == WM_QUIT) {
            breakOrbit = true;
            break;
        }
    }

    return breakOrbit;
}

HWND Win32Window::createWindow(LPCWSTR className, LPCWSTR windowName, WNDPROC windowProc) {
    WNDCLASSEX wcex = { };
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = windowProc;
    wcex.hInstance = this->instance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = HBRUSH(COLOR_WINDOW);
    wcex.lpszClassName = className;

    if (!RegisterClassEx(&wcex)) {
        throw std::runtime_error(LogFormat("RegisterClassEx()"));
    }

    HWND window = CreateWindow(
            className, windowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
            CW_USEDEFAULT, CW_USEDEFAULT, this->width, this->height,
            nullptr, nullptr, this->instance, nullptr);
    if (window == nullptr) {
        throw std::runtime_error(LogFormat("CreateWindow()"));
    }

    return window;
}

void Win32Window::destroyWindow(HWND window) {
    WCHAR className[64];

    if (window != nullptr) {
        GetClassName(window, className, 64);
        DestroyWindow(window);
    }

    UnregisterClass(className, this->instance);
}

void Win32Window::createContext() {
    HWND dummyWindow = this->createWindow(L"Dummy Class", L"Dummy Window", DefWindowProc);
    this->createBaseContext(dummyWindow);

    OpenGL::loadWglExtensions();

    this->destroyContext();
    this->destroyWindow(dummyWindow);

    this->createExtContext(this->window);
}

void Win32Window::createBaseContext(HWND window) {
    this->deviceContext = GetDC(window);
    if (this->deviceContext == nullptr) {
        throw std::runtime_error(LogFormat("GetDC()"));
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
        throw std::runtime_error(LogFormat("ChoosePixelFormat()"));
    }

    if (!SetPixelFormat(this->deviceContext, pixelFormat, &pfd)) {
        throw std::runtime_error(LogFormat("SetPixelFormat()"));
    }

    this->renderingContext = wglCreateContext(this->deviceContext);
    if (this->renderingContext == nullptr) {
        throw std::runtime_error(LogFormat("wglCreateContext()"));
    }

    if (!wglMakeCurrent(this->deviceContext, this->renderingContext)) {
        throw std::runtime_error(LogFormat("wglMakeCurrent()"));
    }
}

void Win32Window::createExtContext(HWND window) {
    this->deviceContext = GetDC(window);
    if (this->deviceContext == nullptr) {
        throw std::runtime_error(LogFormat("GetDC()"));
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
        throw std::runtime_error(LogFormat("wglChoosePixelFormatARB()"));
    }

    PIXELFORMATDESCRIPTOR pfd;
    if (!SetPixelFormat(this->deviceContext, pixelFormat, &pfd)) {
        throw std::runtime_error(LogFormat("SetPixelFormat()"));
    }

    const int contextAttribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    };

    this->renderingContext = wglCreateContextAttribsARB(this->deviceContext, nullptr, contextAttribList);
    if (this->renderingContext == nullptr) {
        throw std::runtime_error(LogFormat("wglCreateContextAttribsARB()"));
    }

    if (!wglMakeCurrent(this->deviceContext, this->renderingContext)) {
        throw std::runtime_error(LogFormat("wglMakeCurrent()"));
    }
}

void Win32Window::destroyContext() {
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

void Win32Window::warpMousePointer(int x, int y) {
    POINT position = { x, y };
    ClientToScreen(this->window, &position);
    SetCursorPos(position.x, position.y);
}

}  // namespace Graphene

#endif  // defined(_WIN32)