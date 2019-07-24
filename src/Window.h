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
#include <GeometryBuffer.h>
#include <Viewport.h>
#include <Overlay.h>
#include <Signals.h>
#include <array>
#include <utility>
#include <string>
#include <unordered_set>
#include <unordered_map>

namespace Graphene {

enum MouseButton {
    LEFT,
    MIDDLE,
    RIGHT,
    X1,     // Wheel up
    X2      // Wheel down
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
    GRAPHENE_API bool isExtensionSupported(const std::string& extension);

    GRAPHENE_API virtual void captureMouse(bool captured) = 0;
    GRAPHENE_API virtual void setVsync(bool vsync) = 0;
    GRAPHENE_API virtual bool dispatchEvents() = 0;

    GRAPHENE_API const std::unordered_set<std::shared_ptr<Overlay>>& getOverlays() const;
    GRAPHENE_API std::shared_ptr<Overlay> createOverlay(int left, int top, int width, int height);

    GRAPHENE_API std::shared_ptr<Viewport> createViewport(int left, int top, int width, int height) override;
    GRAPHENE_API void update() override;

protected:
    virtual std::string getExtensions() = 0;
    virtual void swapBuffers() = 0;

    friend class Engine;
    Signals::Signal<int, int> onMouseMotionSignal;
    Signals::Signal<int, bool> onMouseButtonSignal;
    Signals::Signal<int, bool> onKeyboardButtonSignal;

    KeyboardState keyboardState = { };
    MouseState mouseState = { };
    MousePosition mousePosition = { };
    bool mouseCaptured = false;

    std::unordered_set<std::string> availableExtensions;

    std::unordered_map<std::shared_ptr<Viewport>, std::shared_ptr<GeometryBuffer>> geometryBuffers;
    std::unordered_set<std::shared_ptr<Overlay>> overlays;
};

}  // namespace Graphene

#endif  // WINDOW_H
