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
#include <Input.h>
#include <RenderTarget.h>
#include <GeometryBuffer.h>
#include <Viewport.h>
#include <Overlay.h>
#include <Signals.h>
#include <string>
#include <unordered_set>
#include <unordered_map>

namespace Graphene {

class Window: public RenderTarget {
public:
    GRAPHENE_API Window(int width, int height);
    GRAPHENE_API virtual ~Window() = default;

    GRAPHENE_API const KeyboardState& getKeyboardState() const;
    GRAPHENE_API const MouseState& getMouseState() const;
    GRAPHENE_API const MousePosition& getMousePosition() const;
    GRAPHENE_API bool isMouseCaptured() const;

    GRAPHENE_API bool isExtensionSupported(const std::string& extension) const;
    GRAPHENE_API const std::unordered_set<std::string>& getSupportedExtensions() const;

    GRAPHENE_API virtual void captureMouse(bool captured) = 0;
    GRAPHENE_API virtual void setVsync(bool vsync) = 0;
    GRAPHENE_API virtual void setFullscreen(bool fullscreen) = 0;

    GRAPHENE_API virtual bool dispatchEvents() = 0;
    GRAPHENE_API virtual void swapBuffers() = 0;

    GRAPHENE_API const std::shared_ptr<Overlay>& createOverlay(int left, int top, int width, int height);
    GRAPHENE_API const std::unordered_set<std::shared_ptr<Overlay>>& getOverlays() const;

    GRAPHENE_API const std::shared_ptr<Viewport>& createViewport(int left, int top, int width, int height) override;
    GRAPHENE_API void update() override;

protected:
    friend class Engine;
    Signals::Signal<int, int> onMouseMotionSignal;
    Signals::Signal<MouseButton, bool> onMouseButtonSignal;
    Signals::Signal<KeyboardKey, bool> onKeyboardKeySignal;

    KeyboardState keyboardState = { };
    MouseState mouseState = { };
    MousePosition mousePosition = { };

    bool mouseCaptured = false;
    bool fullscreen = false;

    std::unordered_set<std::string> availableExtensions;

    std::unordered_map<std::shared_ptr<Viewport>, std::shared_ptr<GeometryBuffer>> geometryBuffers;
    std::unordered_set<std::shared_ptr<Overlay>> overlays;
};

}  // namespace Graphene

#endif  // WINDOW_H
