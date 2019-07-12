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

#if defined(__linux__)

#include <WindowLinux.h>
#include <Logger.h>
#include <RenderTarget.h>
#include <stdexcept>

namespace Graphene {

WindowLinux::WindowLinux(int width, int height):
        Window(width, height) {
    this->createWindow("OpenGL Window");
    this->createContext();
}

WindowLinux::~WindowLinux() {
    this->destroyContext();
    this->destroyWindow();
}

void WindowLinux::captureMouse(bool captured) {
    this->mouseCaptured = captured;

    if (this->mouseCaptured) {
        char data[8] = { };
        Pixmap pixmap = XCreateBitmapFromData(this->display, this->window, data, 1, 1);

        XColor color = { };
        Cursor cursor = XCreatePixmapCursor(this->display, pixmap, pixmap, &color, &color, 0, 0);

        XFreePixmap(this->display, pixmap);
        XDefineCursor(this->display, this->window, cursor);

        this->mousePosition = std::make_pair(this->width >> 1, this->height >> 1);
        XWarpPointer(this->display, None, this->window, 0, 0, 0, 0, this->mousePosition.first, this->mousePosition.second);
    } else {
        XUndefineCursor(this->display, this->window);
    }
}

void WindowLinux::update() {
    RenderTarget::update();
    glXSwapBuffers(this->display, this->window);
}

bool WindowLinux::dispatchEvents() {
    XEvent event;
    bool breakOrbit = false;

    while (XPending(this->display)) {
        XNextEvent(this->display, &event);

        switch (event.type) {
            case KeyPress:
            case KeyRelease: {
                bool keyState = (event.type == KeyPress);
                this->keyboardState[event.xkey.keycode] = keyState;
                this->onKeyboardButtonSignal(event.xkey.keycode, keyState);
                break;
            }

            case ButtonPress:
            case ButtonRelease: {
                MouseButton mouseButton = static_cast<MouseButton>(event.xbutton.button);
                bool mouseButtonState = (event.type == ButtonPress);
                this->mouseState[mouseButton] = mouseButtonState;
                this->onMouseButtonSignal(mouseButton, mouseButtonState);
                break;
            }

            case MotionNotify: {
                int xPosition = event.xmotion.x;
                int yPosition = event.xmotion.y;
                bool mouseMoved = true;

                if (this->mouseCaptured) {
                    std::pair<int, int> windowCenter = { this->width >> 1, this->height >> 1 };
                    xPosition -= windowCenter.first;
                    yPosition -= windowCenter.second;

                    if (xPosition != 0 || yPosition != 0) {
                        XWarpPointer(this->display, None, this->window, 0, 0, 0, 0, windowCenter.first, windowCenter.second);
                    } else {
                        mouseMoved = false;  // MotionNotify after XWarpPointer() to window center
                    }
                }

                if (mouseMoved) {
                    this->mousePosition = std::make_pair(xPosition, yPosition);
                    this->onMouseMotionSignal(xPosition, yPosition);
                }

                break;
            }

            case ClientMessage:
                if (event.xclient.data.l[0] == (long)this->wmDeleteWindow) {
                    breakOrbit = true;
                }

                break;

            default:
                break;
        }
    }

    return breakOrbit;
}

void WindowLinux::createWindow(const char* windowName) {
    this->display = XOpenDisplay(nullptr);
    if (this->display == nullptr) {
        throw std::runtime_error(LogFormat("XOpenDisplay()"));
    }

    int screen = XDefaultScreen(this->display);
    XID rootWindow = XRootWindow(this->display, screen);

    const int fbAttribList[] = {
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, True,
        GLX_X_RENDERABLE, True,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        GLX_STENCIL_SIZE, 8,
        None
    };

    int fbNumberConfigs;
    GLXFBConfig* fbConfigs = glXChooseFBConfig(this->display, screen, fbAttribList, &fbNumberConfigs);
    if (fbConfigs == nullptr) {
        throw std::runtime_error(LogFormat("glXChooseFBConfig()"));
    }

    this->fbConfig = fbConfigs[0];
    XFree(fbConfigs);

    XVisualInfo* visualInfo = glXGetVisualFromFBConfig(this->display, this->fbConfig);
    this->colormap = XCreateColormap(this->display, rootWindow, visualInfo->visual, AllocNone);
    if (this->colormap == None) {
        throw std::runtime_error(LogFormat("XCreateColormap()"));
    }

    XSetWindowAttributes windowAttributes = { };
    windowAttributes.colormap = this->colormap;
    windowAttributes.event_mask = PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

    this->window = XCreateWindow(
            this->display, rootWindow, 0, 0, this->width, this->height, 0,
            visualInfo->depth, InputOutput, visualInfo->visual, CWColormap | CWEventMask, &windowAttributes);
    if (this->window == None) {
        throw std::runtime_error(LogFormat("XCreateWindow()"));
    }

    XStoreName(this->display, this->window, windowName);
    XMapRaised(this->display, this->window);
    XFree(visualInfo);

    // This actually registers WM_DELETE_WINDOW so we can intercept it
    this->wmDeleteWindow = XInternAtom(this->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(this->display, this->window, &this->wmDeleteWindow, 1);
}

void WindowLinux::destroyWindow() {
    if (this->display != nullptr) {
        if (this->window != None) {
            XDestroyWindow(this->display, this->window);
            this->window = None;
        }

        if (this->colormap != None) {
            XFreeColormap(this->display, this->colormap);
            this->colormap = None;
        }

        XCloseDisplay(this->display);
        this->display = nullptr;
    }
}

void WindowLinux::createContext() {
    OpenGL::loadGlxExtensions();

    const int contextAttribList[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | GLX_CONTEXT_DEBUG_BIT_ARB,
        None
    };

    this->renderingContext = glXCreateContextAttribsARB(this->display, this->fbConfig, nullptr, True, contextAttribList);
    if (this->renderingContext == nullptr) {
        throw std::runtime_error(LogFormat("glXCreateContextAttribsARB()"));
    }

    if (!glXIsDirect(this->display, this->renderingContext)) {
        throw std::runtime_error(LogFormat("glXIsDirect()"));
    }

    if (!glXMakeCurrent(this->display, this->window, this->renderingContext)) {
        throw std::runtime_error(LogFormat("glXMakeCurrent()"));
    }
}

void WindowLinux::destroyContext() {
    if (this->display != nullptr) {
        glXMakeCurrent(this->display, None, None);

        if (this->renderingContext != nullptr) {
            glXDestroyContext(this->display, this->renderingContext);
            this->renderingContext = nullptr;
        }
    }
}

}  // namespace Graphene

#endif  // defined(__linux__)
