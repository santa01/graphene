#pragma once

#include "stdafx.h"

class Window
{
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

    const LPCWSTR mWindowClassName = L"OpenGL Window Class";
    const LPCWSTR mWindowName = L"OpenGL Window";

    int mWidth = 0;
    int mHeight = 0;

    HINSTANCE mInstance = nullptr;
    HWND mWindow = nullptr;
    HDC mDeviceContext = nullptr;
    HGLRC mRenderingContext = nullptr;
};
