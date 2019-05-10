#pragma once

#include <windows.h>

namespace Graphene {

class Window {
public:
    Window(int width, int height);
    ~Window();

    void createRenderingContext();
    void createRenderingContextExt();
    void destroyRenderingContext();

    void show();
    bool processEvents();

private:
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
