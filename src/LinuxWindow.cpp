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

#include <LinuxWindow.h>
#include <Input.h>
#include <Logger.h>
#include <RenderTarget.h>
#include <sstream>
#include <stdexcept>
#include <X11/extensions/Xrandr.h>

namespace Graphene {

LinuxWindow::LinuxWindow(int width, int height):
        Window(width, height) {
    this->createWindow("OpenGL Window");
    this->createKeyboardMapping();
    this->createContext();
}

LinuxWindow::~LinuxWindow() {
    this->setFullscreen(false);

    this->destroyContext();
    this->destroyKeyboardMapping();
    this->destroyWindow();
}

void LinuxWindow::captureMouse(bool captured) {
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

void LinuxWindow::setVsync(bool vsync) {
    if (this->isExtensionSupported("GLX_EXT_swap_control")) {
        bool adaptive = this->isExtensionSupported("GLX_EXT_swap_control_tear");
        if (vsync && adaptive) {
            LogInfo("GLX_EXT_swap_control_tear available, enable adaptive vsync");
        }

        glXSwapIntervalEXT(this->display, this->window, vsync ? (adaptive ? -1 : 1) : 0);
    } else if (this->isExtensionSupported("GLX_MESA_swap_control")) {
        LogInfo("GLX_EXT_swap_control unavailable, using GLX_MESA_swap_control instead");

        glXSwapIntervalMESA(vsync ? 1 : 0);
    } else {
        LogWarn("GLX_EXT_swap_control, GLX_MESA_swap_control unavailable, leave vsync as-is");
    }
}

void LinuxWindow::setFullscreen(bool fullscreen) {
    static int previousModeNumber = 0;

    XRRScreenConfiguration* screenConfig = XRRGetScreenInfo(this->display, this->rootWindow);
    if (screenConfig == nullptr) {
        throw std::runtime_error(LogFormat("XRRGetScreenInfo()"));
    }

    /*
     * Per https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html#idm140130317598336
     * _NET_WM_STATE
     *
     * To change the state of a mapped window, a Client MUST send a _NET_WM_STATE client message to the root window:
     *
     *   window = the respective client window
     *   message_type = _NET_WM_STATE
     *   format = 32
     *   data.l[0] = the action, as listed below
     *   data.l[1] = first property to alter
     *   data.l[2] = second property to alter
     *   data.l[3] = source indication
     *   other data.l[] elements = 0
     */
    XClientMessageEvent event = { };
    event.type = ClientMessage;
    event.window = this->window;
    event.message_type = this->wmState;
    event.format = 32;    // XEvent.data should be viewed as a list of longs
    event.data.l[0] = 2;  // _NET_WM_STATE_TOGGLE
    event.data.l[1] = this->wmStateFullscreen;
    event.data.l[3] = 1;  // Source indication: normal application

    /*
     * https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html#idm140130317705584
     * Root Window Properties (and Related Messages)
     *
     * Whenever this spec speaks about “sending a message to the root window”, it is understood
     * that the client is supposed to create a ClientMessage event with the specified contents and
     * send it by using a SendEvent request with the following arguments:
     *
     *  destination     root
     *  propagate       False
     *  event-mask      (SubstructureNotify|SubstructureRedirect)
     *  event           the specified ClientMessage
     */
    long eventMask = SubstructureNotifyMask | SubstructureRedirectMask;

    if (fullscreen && !this->fullscreen) {
        int modeNumber = 0;
        bool resolutionSupported = false;

        int totalModes;
        XRRScreenSize* screenSizes = XRRConfigSizes(screenConfig, &totalModes);
        if (screenSizes == nullptr) {
            throw std::runtime_error(LogFormat("XRRConfigSizes()"));
        }

        for (modeNumber = 0; modeNumber < totalModes; modeNumber++) {
            if (screenSizes[modeNumber].width != this->width || screenSizes[modeNumber].height != this->height) {
                continue;
            }

            resolutionSupported = true;
            break;
        }

        if (resolutionSupported) {
            // Adjust screen resolution
            Rotation rotation;
            previousModeNumber = XRRConfigCurrentConfiguration(screenConfig, &rotation);
            XRRSetScreenConfig(this->display, screenConfig, this->rootWindow, modeNumber, rotation, CurrentTime);

            // Make window occupy the whole screen
            this->fullscreen = fullscreen;
            XSendEvent(this->display, this->rootWindow, False, eventMask, reinterpret_cast<XEvent*>(&event));
        } else {
            LogWarn("%dx%d resolution is unsupported in fullscreen mode", this->width, this->height);
        }
    } else if (!fullscreen && this->fullscreen) {
        // Restore saved screen resolution
        Rotation rotation;
        XRRConfigCurrentConfiguration(screenConfig, &rotation);
        XRRSetScreenConfig(this->display, screenConfig, this->rootWindow, previousModeNumber, rotation, CurrentTime);

        // Restore window parameters
        this->fullscreen = fullscreen;
        XSendEvent(this->display, this->rootWindow, False, eventMask, reinterpret_cast<XEvent*>(&event));
    }

    XRRFreeScreenConfigInfo(screenConfig);
}

bool LinuxWindow::dispatchEvents() {
    XEvent event;
    bool breakOrbit = false;

    while (XPending(this->display)) {
        XNextEvent(this->display, &event);

        switch (event.type) {
            case KeyPress:
            case KeyRelease: {
                KeySym keysym = this->keycodeToKeysym(event.xkey.keycode);
                KeyboardKey keyboardKey = Input::keycodeToKeyboardKey(static_cast<int>(keysym));

                if (keyboardKey != KeyboardKey::KEY_UNMAPPED) {
                    bool keyState = (event.type == KeyPress);
                    this->keyboardState[keyboardKey] = keyState;
                    this->onKeyboardKeySignal(keyboardKey, keyState);
                }

                break;
            }

            case ButtonPress:
            case ButtonRelease: {
                // XButtonEvent.button values start from 1 (left button)
                // to 5 (x2 button). Scale to MouseButton enum values (0 to 4).
                MouseButton mouseButton = static_cast<MouseButton>(event.xbutton.button - 1);
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
                if (event.xclient.data.l[0] == static_cast<long>(this->wmDeleteWindow)) {
                    breakOrbit = true;
                }

                break;

            default:
                if (event.type - this->xrandrEventBase == RRScreenChangeNotify) {
                    /*
                     * Per http://www.x.org/archive/X11R7.5/doc/man/man3/Xrandr.3.html#sect4
                     * Xlib notification
                     *
                     * Clients must call back into Xlib using XRRUpdateConfiguration when screen configuration
                     * change notify events are generated (or root window configuration changes occur, to update
                     * Xlib's view of the resolution, size, rotation, reflection or subpixel order.
                     */
                    XRRUpdateConfiguration(&event);
                }

                break;
        }
    }

    return breakOrbit;
}

void LinuxWindow::swapBuffers() {
    glXSwapBuffers(this->display, this->window);
}

void LinuxWindow::createWindow(const char* windowName) {
    this->display = XOpenDisplay(nullptr);
    if (this->display == nullptr) {
        throw std::runtime_error(LogFormat("XOpenDisplay()"));
    }

    this->screen = XDefaultScreen(this->display);
    this->rootWindow = XRootWindow(this->display, this->screen);

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
    GLXFBConfig* fbConfigs = glXChooseFBConfig(this->display, this->screen, fbAttribList, &fbNumberConfigs);
    if (fbConfigs == nullptr) {
        throw std::runtime_error(LogFormat("glXChooseFBConfig()"));
    }

    this->fbConfig = fbConfigs[0];
    XFree(fbConfigs);

    XVisualInfo* visualInfo = glXGetVisualFromFBConfig(this->display, this->fbConfig);
    this->colormap = XCreateColormap(this->display, this->rootWindow, visualInfo->visual, AllocNone);
    if (this->colormap == None) {
        throw std::runtime_error(LogFormat("XCreateColormap()"));
    }

    XSetWindowAttributes windowAttributes = { };
    windowAttributes.colormap = this->colormap;
    windowAttributes.event_mask = PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

    this->window = XCreateWindow(
            this->display, this->rootWindow, 0, 0, this->width, this->height, 0,
            visualInfo->depth, InputOutput, visualInfo->visual, CWColormap | CWEventMask, &windowAttributes);
    if (this->window == None) {
        throw std::runtime_error(LogFormat("XCreateWindow()"));
    }

    XStoreName(this->display, this->window, windowName);
    XMapRaised(this->display, this->window);
    XFree(visualInfo);

    /*
     * Per https://tronche.com/gui/x/icccm/sec-4.html#s-4.2.8.1
     * 4.2.8.1. Window Deletion
     *
     * Clients receiving a WM_DELETE_WINDOW message should behave as if
     * the user selected "delete window" from a hypothetical menu.
     */
    this->wmDeleteWindow = XInternAtom(this->display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(this->display, this->window, &this->wmDeleteWindow, True);

    /*
     * Per https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html#NORESIZE
     * Fixed size windows
     *
     * Windows can indicate that they are non-resizable by setting minheight = maxheight
     * and minwidth = maxwidth in the ICCCM WM_NORMAL_HINTS property.
     */
    XSizeHints* hints = XAllocSizeHints();
    hints->flags = PMinSize | PMaxSize;
    hints->min_width = hints->max_width = this->width;
    hints->min_height = hints->max_height = this->height;

    XSetWMNormalHints(this->display, this->window, hints);
    XFree(hints);

    /*
     * Per https://specifications.freedesktop.org/wm-spec/wm-spec-1.3.html#idm140130317598336
     * _NET_WM_STATE
     *
     * _NET_WM_STATE_FULLSCREEN indicates that the window should fill the entire screen and have
     * no window decorations. Additionally the Window Manager is responsible for restoring the
     * original geometry after a switch from fullscreen back to normal window.
     */
    this->wmState = XInternAtom(this->display, "_NET_WM_STATE", False);
    this->wmStateFullscreen = XInternAtom(this->display, "_NET_WM_STATE_FULLSCREEN", False);

    /*
     * Per 'man 3 xrandr' manual
     * DATATYPES
     *
     * A XRRScreenChangeNotifyEvent is sent to a client that has requested notification whenever
     * the screen configuration is changed. A client can perform this request by calling XRRSelectInput,
     * passing the display, the root window, and the RRScreenChangeNotifyMask mask.
     */
    int xrandrErrorBase;
    XRRQueryExtension(this->display, &this->xrandrEventBase, &xrandrErrorBase);
    XRRSelectInput(this->display, this->rootWindow, RRScreenChangeNotifyMask);
}

void LinuxWindow::destroyWindow() {
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

void LinuxWindow::createContext() {
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

    std::stringstream extensions(glXQueryExtensionsString(this->display, this->screen));
    std::string extension;

    while (std::getline(extensions, extension, ' ')) {
        this->availableExtensions.insert(extension);
    }
}

void LinuxWindow::destroyContext() {
    if (this->display != nullptr) {
        glXMakeCurrent(this->display, None, None);

        if (this->renderingContext != nullptr) {
            glXDestroyContext(this->display, this->renderingContext);
            this->renderingContext = nullptr;
        }
    }
}

void LinuxWindow::createKeyboardMapping() {
    int maxKeycode;
    XDisplayKeycodes(this->display, &this->firstKeycode, &maxKeycode);

    /*
     * Per https://tronche.com/gui/x/xlib/input/XGetKeyboardMapping.html
     * Description:
     *
     * In addition, the following expression must be less than or equal to max_keycode
     * as returned by XDisplayKeycodes():
     *
     * first_keycode + keycode_count - 1
     */
    int keycodeCount = maxKeycode - this->firstKeycode + 1;
    this->keysymsMapping = XGetKeyboardMapping(this->display, this->firstKeycode, keycodeCount, &this->keysymsPerKeycode);
}

void LinuxWindow::destroyKeyboardMapping() {
    if (this->display != nullptr) {
        if (this->keysymsMapping != nullptr) {
            XFree(this->keysymsMapping);
            this->keysymsMapping = nullptr;
        }
    }
}

KeySym LinuxWindow::keycodeToKeysym(KeyCode keycode) const {
    /*
     * Per https://tronche.com/gui/x/xlib/input/XGetKeyboardMapping.html
     * Description:
     *
     * KeySym number N, counting from zero, for KeyCode K has the following index in the list,
     * counting from zero:
     *
     * (K - first_code) * keysyms_per_code_return + N
     */
    int index = 0;  // Shift/NumLock modifier released, 1 for a keysym of the Mod+Key combination
    return this->keysymsMapping[(keycode - this->firstKeycode) * this->keysymsPerKeycode + index];
}

}  // namespace Graphene

#endif  // defined(__linux__)
