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

#ifndef LINUXWINDOW_H
#define LINUXWINDOW_H

#if defined(__linux__)

#include <GrapheneApi.h>
#include <Window.h>
#include <OpenGL.h>
#include <string>

namespace Graphene {

class LinuxWindow: public Window {
public:
    GRAPHENE_API LinuxWindow(int width, int height);
    GRAPHENE_API ~LinuxWindow();

    GRAPHENE_API void captureMouse(bool captured) override;
    GRAPHENE_API void setVsync(bool vsync) override;
    GRAPHENE_API void setFullscreen(bool fullscreen) override;

    GRAPHENE_API bool dispatchEvents() override;
    GRAPHENE_API void swapBuffers() override;

private:
    void createWindow(const char* windowName);
    void destroyWindow();

    void createContext();
    void destroyContext();

    void createKeyboardMapping();
    void destroyKeyboardMapping();
    KeySym keycodeToKeysym(KeyCode keycode) const;

    Display* display = nullptr;
    int screen = 0;
    GLXFBConfig fbConfig = nullptr;
    Colormap colormap = None;
    XID window = None;
    Atom wmDeleteWindow = None;
    GLXContext renderingContext = nullptr;

    int firstKeycode = 0;
    int keysymsPerKeycode = 0;
    KeySym* keysymsMapping = nullptr;
};

}  // namespace Graphene

#endif  // defined(__linux__)

#endif  // LINUXWINDOW_H
