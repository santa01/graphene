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

#include <RenderTarget.h>
#include <OpenGL.h>
#include <Signals.h>
#include <array>
#include <utility>

namespace Graphene {

enum MouseButton { LEFT, MIDDLE, RIGHT, X1, X2 };

constexpr int KeyboardScancodes = 256;
constexpr int MouseButtons = 5;

using KeyboardState = std::array<bool, KeyboardScancodes>;
using MouseState = std::array<bool, MouseButtons>;
using MousePosition = std::pair<int, int>;

class WindowWin32: public RenderTarget {
public:
    WindowWin32(int width, int height);
    ~WindowWin32();

    const KeyboardState& getKeyboardState() const;
    const MouseState& getMouseState() const;
    const MousePosition& getMousePosition() const;

    bool isMouseCaptured() const;
    void captureMouse(bool capture);

    void update();
    void dispatchEvents();

    Signals::Signal<int, int> onMouseMotion;
    Signals::Signal<int, bool> onMouseButton;
    Signals::Signal<int, bool> onKeyboardButton;
    Signals::Signal<> onQuit;  // User-requested quit
    Signals::Signal<> onIdle;  // Empty event queue

private:
    friend LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

    void setKeyboardState(int scancode, bool state);
    void setMouseState(MouseButton button, bool state);
    void setMousePosition(int x, int y);
    void warpMousePointer(int x, int y);

    HWND createWindow(LPCWSTR className, LPCWSTR windowName, WNDPROC windowProc);
    void destroyWindow(HWND window);

    void createContext();
    void createBaseContext(HWND window);
    void createExtContext(HWND window);
    void destroyContext();

    HINSTANCE instance = nullptr;
    HWND window = nullptr;
    HDC deviceContext = nullptr;
    HGLRC renderingContext = nullptr;

    bool mouseCaptured = false;

    KeyboardState keyboardState = { };
    MouseState mouseState = { };
    MousePosition mousePosition = { };
};

}  // namespace Graphene

#endif  // defined(_WIN32)

#endif  // WINDOWWIN32_H
