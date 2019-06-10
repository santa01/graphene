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

#ifndef WINDOWWIN32_H
#define WINDOWWIN32_H

#if defined(_WIN32)

#include <Window.h>
#include <OpenGL.h>

namespace Graphene {

class WindowWin32: public Window {
public:
    WindowWin32(int width, int height);
    ~WindowWin32();

    void captureMouse(bool captured) override;
    void update() override;
    void dispatchEvents() override;

private:
    friend LRESULT CALLBACK windowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

    HWND createWindow(LPCWSTR className, LPCWSTR windowName, WNDPROC windowProc);
    void destroyWindow(HWND window);

    void createContext();
    void createBaseContext(HWND window);
    void createExtContext(HWND window);
    void destroyContext();

    void warpMousePointer(int x, int y);

    HINSTANCE instance = nullptr;
    HWND window = nullptr;
    HDC deviceContext = nullptr;
    HGLRC renderingContext = nullptr;
};

}  // namespace Graphene

#endif  // defined(_WIN32)

#endif  // WINDOWWIN32_H
