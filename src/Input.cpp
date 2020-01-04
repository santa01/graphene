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

#include <Input.h>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <X11/keysym.h>
#endif

namespace Graphene {

std::unordered_map<int, KeyboardKey> Input::keyboardMapping = {
#if defined(_WIN32)
    { VK_BACK,          KEY_BACKSPACE },
    { VK_RETURN,        KEY_ENTER },
    { VK_TAB,           KEY_TAB },
    { VK_OEM_3,         KEY_TILDE },
    { VK_SPACE,         KEY_SPACE },
    { VK_SHIFT,         KEY_SHIFT },
    { VK_CONTROL,       KEY_CONTROL },
    { VK_MENU,          KEY_ALT },
    { VK_CAPITAL,       KEY_CAPSLOCK },
    { VK_NUMLOCK,       KEY_NUMLOCK },
    { VK_ESCAPE,        KEY_ESCAPE },
    { VK_PRIOR,         KEY_PAGEUP },
    { VK_NEXT,          KEY_PAGEDOWN },
    { VK_END,           KEY_END },
    { VK_HOME,          KEY_HOME },
    { VK_LEFT,          KEY_LEFT },
    { VK_UP,            KEY_UP },
    { VK_RIGHT,         KEY_RIGHT },
    { VK_DOWN,          KEY_DOWN },
    { VK_INSERT,        KEY_INSERT },      { VK_SEPARATOR,      KEY_INSERT },
    { VK_DELETE,        KEY_DELETE },      { VK_DECIMAL,        KEY_DELETE },
    { VK_MULTIPLY,      KEY_ASTERISK },
    { VK_ADD,           KEY_PLUS },
    { VK_SUBTRACT,      KEY_MINUS },
    { VK_DIVIDE,        KEY_SLASH },
    { 0x30,             KEY_0 },           { VK_NUMPAD0,        KEY_0 },
    { 0x31,             KEY_1 },           { VK_NUMPAD1,        KEY_1 },
    { 0x32,             KEY_2 },           { VK_NUMPAD2,        KEY_2 },
    { 0x33,             KEY_3 },           { VK_NUMPAD3,        KEY_3 },
    { 0x34,             KEY_4 },           { VK_NUMPAD4,        KEY_4 },
    { 0x35,             KEY_5 },           { VK_NUMPAD5,        KEY_5 },
    { 0x36,             KEY_6 },           { VK_NUMPAD6,        KEY_6 },
    { 0x37,             KEY_7 },           { VK_NUMPAD7,        KEY_7 },
    { 0x38,             KEY_8 },           { VK_NUMPAD8,        KEY_8 },
    { 0x39,             KEY_9 },           { VK_NUMPAD9,        KEY_9 },
    { 0x41,             KEY_A },
    { 0x42,             KEY_B },
    { 0x43,             KEY_C },
    { 0x44,             KEY_D },
    { 0x45,             KEY_E },
    { 0x46,             KEY_F },
    { 0x47,             KEY_G },
    { 0x48,             KEY_H },
    { 0x49,             KEY_I },
    { 0x4A,             KEY_J },
    { 0x4B,             KEY_K },
    { 0x4C,             KEY_L },
    { 0x4D,             KEY_M },
    { 0x4E,             KEY_N },
    { 0x4F,             KEY_O },
    { 0x50,             KEY_P },
    { 0x51,             KEY_Q },
    { 0x52,             KEY_R },
    { 0x53,             KEY_S },
    { 0x54,             KEY_T },
    { 0x55,             KEY_U },
    { 0x56,             KEY_V },
    { 0x57,             KEY_W },
    { 0x58,             KEY_X },
    { 0x59,             KEY_Y },
    { 0x5A,             KEY_Z },
    { VK_F1,            KEY_F1 },
    { VK_F2,            KEY_F2 },
    { VK_F3,            KEY_F3 },
    { VK_F4,            KEY_F4 },
    { VK_F5,            KEY_F5 },
    { VK_F6,            KEY_F6 },
    { VK_F7,            KEY_F7 },
    { VK_F8,            KEY_F8 },
    { VK_F9,            KEY_F9 },
    { VK_F10,           KEY_F10 },
    { VK_F11,           KEY_F11 },
    { VK_F12,           KEY_F12 },
#elif defined(__linux__)
    { XK_BackSpace,     KEY_BACKSPACE },
    { XK_Return,        KEY_ENTER },        { XK_KP_Enter,      KEY_ENTER },
    { XK_Tab,           KEY_TAB },
    { XK_grave,         KEY_TILDE },
    { XK_space,         KEY_SPACE },
    { XK_Shift_L,       KEY_SHIFT },        { XK_Shift_R,       KEY_SHIFT },
    { XK_Control_L,     KEY_CONTROL },      { XK_Control_R,     KEY_CONTROL },
    { XK_Alt_L,         KEY_ALT },          { XK_Alt_R,         KEY_ALT },
    { XK_Caps_Lock,     KEY_CAPSLOCK },
    { XK_Num_Lock,      KEY_NUMLOCK },
    { XK_Escape,        KEY_ESCAPE },
    { XK_Page_Up,       KEY_PAGEUP },       { XK_KP_Page_Up,    KEY_PAGEUP },
    { XK_Page_Down,     KEY_PAGEDOWN },     { XK_KP_Page_Down,  KEY_PAGEDOWN },
    { XK_End,           KEY_END },          { XK_KP_End,        KEY_END },
    { XK_Home,          KEY_HOME },         { XK_KP_Home,       KEY_HOME },
    { XK_Left,          KEY_LEFT },         { XK_KP_Left,       KEY_LEFT },
    { XK_Up,            KEY_UP },           { XK_KP_Up,         KEY_UP },
    { XK_Right,         KEY_RIGHT },        { XK_KP_Right,      KEY_RIGHT },
    { XK_Down,          KEY_DOWN },         { XK_KP_Down,       KEY_DOWN },
    { XK_Insert,        KEY_INSERT },       { XK_KP_Insert,     KEY_INSERT },
    { XK_Delete,        KEY_DELETE },       { XK_KP_Delete,     KEY_DELETE },
    { XK_KP_Multiply,   KEY_ASTERISK },
    { XK_KP_Add,        KEY_PLUS },
    { XK_KP_Subtract,   KEY_MINUS },
    { XK_KP_Divide,     KEY_SLASH },
    { XK_0,             KEY_0 },            { XK_KP_0,          KEY_0 },
    { XK_1,             KEY_1 },            { XK_KP_1,          KEY_1 },
    { XK_2,             KEY_2 },            { XK_KP_2,          KEY_2 },
    { XK_3,             KEY_3 },            { XK_KP_3,          KEY_3 },
    { XK_4,             KEY_4 },            { XK_KP_4,          KEY_4 },
    { XK_5,             KEY_5 },            { XK_KP_5,          KEY_5 },
    { XK_6,             KEY_6 },            { XK_KP_6,          KEY_6 },
    { XK_7,             KEY_7 },            { XK_KP_7,          KEY_7 },
    { XK_8,             KEY_8 },            { XK_KP_8,          KEY_8 },
    { XK_9,             KEY_9 },            { XK_KP_9,          KEY_9 },
    { XK_a,             KEY_A },
    { XK_b,             KEY_B },
    { XK_c,             KEY_C },
    { XK_d,             KEY_D },
    { XK_e,             KEY_E },
    { XK_f,             KEY_F },
    { XK_g,             KEY_G },
    { XK_h,             KEY_H },
    { XK_i,             KEY_I },
    { XK_j,             KEY_J },
    { XK_k,             KEY_K },
    { XK_l,             KEY_L },
    { XK_m,             KEY_M },
    { XK_n,             KEY_N },
    { XK_o,             KEY_O },
    { XK_p,             KEY_P },
    { XK_q,             KEY_Q },
    { XK_r,             KEY_R },
    { XK_s,             KEY_S },
    { XK_t,             KEY_T },
    { XK_u,             KEY_U },
    { XK_v,             KEY_V },
    { XK_w,             KEY_W },
    { XK_x,             KEY_X },
    { XK_y,             KEY_Y },
    { XK_z,             KEY_Z },
    { XK_F1,            KEY_F1 },
    { XK_F2,            KEY_F2 },
    { XK_F3,            KEY_F3 },
    { XK_F4,            KEY_F4 },
    { XK_F5,            KEY_F5 },
    { XK_F6,            KEY_F6 },
    { XK_F7,            KEY_F7 },
    { XK_F8,            KEY_F8 },
    { XK_F9,            KEY_F9 },
    { XK_F10,           KEY_F10 },
    { XK_F11,           KEY_F11 },
    { XK_F12,           KEY_F12 },
#endif
};

KeyboardKey Input::keycodeToKeyboardKey(int keycode) {
    auto keyboardKeyIt = Input::keyboardMapping.find(keycode);
    if (keyboardKeyIt == Input::keyboardMapping.end()) {
        return KeyboardKey::KEY_UNKNOWN;
    }

    return keyboardKeyIt->second;
}

}  // namespace Graphene
