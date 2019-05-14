#pragma once

#include <windows.h>
#include <vector>
#include <utility>

namespace Graphene {

class Window {
public:
    Window(int width, int height);
    ~Window();

    int getWidth() const;
    int getHeight() const;

    void createContext();
    void destroyContext();

    std::vector<bool> getKeyboardState() const;
    std::vector<bool> getMouseState() const;
    std::pair<int, int> getMousePosition() const;

    bool isMouseCaptured() const;
    void captureMouse(bool capture);

    void update();
    bool dispatchEvents();

private:
    HWND createWindow(LPCWSTR className, LPCWSTR windowName, WNDPROC windowProc);
    void destroyWindow(HWND window);

    void createBaseContext(HWND window);
    void createExtContext(HWND window);

    std::vector<bool> getVirtualKeysState(int startKey, int endKey) const;

    int width = 0;
    int height = 0;
    bool mouseCaptured = false;

    HINSTANCE instance = nullptr;
    HWND window = nullptr;
    HDC deviceContext = nullptr;
    HGLRC renderingContext = nullptr;
};

}  // namespace Graphene
