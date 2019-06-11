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

#ifndef WINDOW_H
#define WINDOW_H

#include <GrapheneApi.h>
#include <RenderTarget.h>
#include <Signals.h>
#include <array>
#include <utility>

namespace Graphene {

enum MouseButton {
    LEFT,
    MIDDLE,
    RIGHT,
    X1,
    X2
};

constexpr int KeyboardScancodes = 256;
constexpr int MouseButtons = 5;

typedef std::array<bool, KeyboardScancodes> KeyboardState;
typedef std::array<bool, MouseButtons> MouseState;
typedef std::pair<int, int> MousePosition;

class Window: public RenderTarget {
public:
    GRAPHENE_API Window(int width, int height);
    GRAPHENE_API virtual ~Window() = default;

    GRAPHENE_API const KeyboardState& getKeyboardState() const;
    GRAPHENE_API const MouseState& getMouseState() const;
    GRAPHENE_API const MousePosition& getMousePosition() const;
    GRAPHENE_API bool isMouseCaptured() const;

    GRAPHENE_API virtual void captureMouse(bool captured) = 0;
    GRAPHENE_API virtual void update() = 0;
    GRAPHENE_API virtual void dispatchEvents() = 0;

    Signals::Signal<int, int> onMouseMotion;
    Signals::Signal<int, bool> onMouseButton;
    Signals::Signal<int, bool> onKeyboardButton;
    Signals::Signal<> onQuit;  // User-requested quit
    Signals::Signal<> onIdle;  // Empty event queue

protected:
    void setKeyboardState(int scancode, bool state);
    void setMouseState(MouseButton button, bool state);
    void setMousePosition(int x, int y);
    void setMouseCaptured(bool capture);

    KeyboardState keyboardState;
    MouseState mouseState;
    MousePosition mousePosition;
    bool mouseCaptured = false;
};

}  // namespace Graphene

#endif  // WINDOW_H
