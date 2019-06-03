#pragma once

#include <Signals.h>
#include <windows.h>
#include <array>
#include <utility>

namespace Graphene {

enum MouseButton { LEFT, MIDDLE, RIGHT, X1, X2 };

constexpr int KeyboardScancodes = 256;
constexpr int MouseButtons = 5;

using KeyboardState = std::array<bool, KeyboardScancodes>;
using MouseState = std::array<bool, MouseButtons>;
using MousePosition = std::pair<int, int>;

class Window {
public:
    Window(int width, int height);
    ~Window();

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
    Signals::Signal<> onIdle; // Empty event queue

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

    int width = 0;
    int height = 0;

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
