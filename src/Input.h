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

#ifndef INPUT_H
#define INPUT_H

#include <GrapheneApi.h>
#include <unordered_map>
#include <array>
#include <utility>

namespace Graphene {

enum KeyboardKey {
    KEY_BACKSPACE,
    KEY_ENTER,
    KEY_TAB,
    KEY_TILDE,
    KEY_SPACE,
    KEY_SHIFT,
    KEY_CONTROL,
    KEY_ALT,
    KEY_CAPSLOCK,
    KEY_NUMLOCK,
    KEY_ESCAPE,
    KEY_PAGEUP,
    KEY_PAGEDOWN,
    KEY_END,
    KEY_HOME,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_INSERT,
    KEY_DELETE,
    KEY_ASTERISK,
    KEY_PLUS,
    KEY_MINUS,
    KEY_SLASH,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEYBOARD_KEYS,
    KEY_UNMAPPED = 0xFF
};

enum MouseButton {
    BUTTON_LEFT,
    BUTTON_MIDDLE,
    BUTTON_RIGHT,
    BUTTON_X1,     // Wheel up
    BUTTON_X2,     // Wheel down,
    MOUSE_BUTTONS,
    BUTTON_UNMAPPED = 0xFF
};

typedef std::array<bool, KeyboardKey::KEYBOARD_KEYS> KeyboardState;
typedef std::array<bool, MouseButton::MOUSE_BUTTONS> MouseState;
typedef std::pair<int, int> MousePosition;

class Input {
public:
    GRAPHENE_API static KeyboardKey keycodeToKeyboardKey(int keycode);

private:
    static std::unordered_map<int, KeyboardKey> keyboardMapping;
};

}  // namespace Graphene

#endif  // INPUT_H
