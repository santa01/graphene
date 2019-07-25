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

#ifndef WIN32WINDOW_H
#define WIN32WINDOW_H

#if defined(_WIN32)

#include <GrapheneApi.h>
#include <Window.h>
#include <OpenGL.h>
#include <string>

namespace Graphene {

class Win32Window: public Window {
public:
    GRAPHENE_API Win32Window(int width, int height);
    GRAPHENE_API ~Win32Window();

    GRAPHENE_API void captureMouse(bool captured) override;
    GRAPHENE_API void setVsync(bool vsync) override;
    GRAPHENE_API bool dispatchEvents() override;
    GRAPHENE_API void swapBuffers() override;

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

#endif  // WIN32WINDOW_H
