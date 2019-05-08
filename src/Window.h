#pragma once

#include <stdafx.h>

namespace Graphene {

class Window {
public:
    Window(int width, int height, HINSTANCE instance);
    ~Window();

    void createRenderingContext();
    void createRenderingContextARB();
    void destroyRenderingContext();

    void show();
    void processEvents(bool oneShot);

private:
    void createConsole();
    void destroyConsole();

    void createWindow();
    void destroyWindow();

    const LPCWSTR windowClassName = L"OpenGL Window Class";
    const LPCWSTR windowName = L"OpenGL Window";

    int width = 0;
    int height = 0;

    HINSTANCE instance = nullptr;
    HWND window = nullptr;
    HDC deviceContext = nullptr;
    HGLRC renderingContext = nullptr;
};

}  // namespace Graphene
