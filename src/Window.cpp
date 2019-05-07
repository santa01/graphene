#include <stdexcept>

#include "Window.h"
#include "OpenGL.h"

LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(window, message, wParam, lParam);
}

Window::Window(int width, int height, HINSTANCE instance) :
    mWidth(width),
    mHeight(height),
    mInstance(instance)
{
    createConsole();
}

Window::~Window()
{
    destroyConsole();
}

void Window::createRenderingContext()
{
    createWindow();

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixelFormat = ChoosePixelFormat(mDeviceContext, &pfd);
    if (pixelFormat == 0)
    {
        throw std::runtime_error("ChoosePixelFormat()");
    }

    if (!SetPixelFormat(mDeviceContext, pixelFormat, &pfd))
    {
        throw std::runtime_error("SetPixelFormat()");
    }

    mRenderingContext = wglCreateContext(mDeviceContext);
    if (mRenderingContext == nullptr)
    {
        throw std::runtime_error("wglCreateContext()");
    }

    if (!wglMakeCurrent(mDeviceContext, mRenderingContext))
    {
        throw std::runtime_error("wglMakeCurrent()");
    }
}

void Window::createRenderingContextARB()
{
    createWindow();

    const int pixelAttribList[] =
    {
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

    if (!OpenGL::wglChoosePixelFormatARB(mDeviceContext, pixelAttribList, nullptr, 1, &pixelFormat, &numberFormats))
    {
        throw std::runtime_error("wglChoosePixelFormatARB()");
    }

    if (!SetPixelFormat(mDeviceContext, pixelFormat, &pfd))
    {
        throw std::runtime_error("SetPixelFormat()");
    }

    const int contextAttribList[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 0,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    mRenderingContext = OpenGL::wglCreateContextAttribsARB(mDeviceContext, nullptr, contextAttribList);
    if (mRenderingContext == nullptr)
    {
        throw std::runtime_error("wglCreateContextAttribsARB()");
    }

    if (!wglMakeCurrent(mDeviceContext, mRenderingContext))
    {
        throw std::runtime_error("wglMakeCurrent()");
    }
}

void Window::destroyRenderingContext()
{
    if (mDeviceContext != nullptr)
    {
        wglMakeCurrent(mDeviceContext, nullptr);
    }

    if (mRenderingContext != nullptr)
    {
        wglDeleteContext(mRenderingContext);
        mRenderingContext = nullptr;
    }

    destroyWindow();
}

void Window::show()
{
    ShowWindow(mWindow, SW_SHOWDEFAULT);
    UpdateWindow(mWindow);
}

void Window::processEvents(bool oneShot)
{
    MSG message;
    bool breakOrbit = false;

    while (!breakOrbit && GetMessage(&message, mWindow, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);

        breakOrbit = oneShot;
    }
}

void Window::createConsole()
{
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    FILE* stream;
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);
}

void Window::destroyConsole()
{
    FreeConsole();
}

void Window::createWindow()
{
    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = windowProc;
    wcex.hInstance = mInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = HBRUSH(COLOR_WINDOW);
    wcex.lpszClassName = mWindowClassName;

    if (!RegisterClassEx(&wcex))
    {
        throw std::runtime_error("RegisterClassEx()");
    }

    mWindow = CreateWindow(
        mWindowClassName, mWindowName, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, mWidth, mHeight, nullptr, nullptr, mInstance, nullptr);
    if (mWindow == nullptr)
    {
        throw std::runtime_error("CreateWindow()");
    }

    mDeviceContext = GetDC(mWindow);
    if (mDeviceContext == nullptr)
    {
        throw std::runtime_error("GetDC()");
    }
}

void Window::destroyWindow()
{
    if (mWindow != nullptr)
    {
        if (mDeviceContext != nullptr)
        {
            ReleaseDC(mWindow, mDeviceContext);
            mDeviceContext = nullptr;
        }

        DestroyWindow(mWindow);
        mWindow = nullptr;
    }

    UnregisterClass(mWindowClassName, mInstance);
}
