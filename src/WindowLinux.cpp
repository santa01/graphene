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
#include <stdexcept>
#include <iostream>
#include <string>
#include <unordered_map>

namespace Graphene {

std::unordered_map<GLenum, std::string> sourceMap = {
    { GL_DEBUG_SOURCE_API_ARB,             "OpenGL" },
    { GL_DEBUG_SOURCE_SHADER_COMPILER_ARB, "GLSL" },
    { GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB,   "Window system" },
    { GL_DEBUG_SOURCE_THIRD_PARTY_ARB,     "3rd party" },
    { GL_DEBUG_SOURCE_APPLICATION_ARB,     "Application" },
    { GL_DEBUG_SOURCE_OTHER_ARB,           "Other" }
};

std::unordered_map<GLenum, std::string> typeMap = {
    { GL_DEBUG_TYPE_ERROR_ARB,               "Error" },
    { GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB, "Deprecated" },
    { GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB,  "Indefined" },
    { GL_DEBUG_TYPE_PERFORMANCE_ARB,         "Performance" },
    { GL_DEBUG_TYPE_PORTABILITY_ARB,         "Portability" },
    { GL_DEBUG_TYPE_OTHER_ARB,               "Other" }
};

extern "C" void debugHandler(GLenum source, GLenum type, GLuint /*id*/, GLenum /*severity*/,
        GLsizei /*length*/, const GLchar* message, const GLvoid* /*userParam*/) {
    std::cout << typeMap[type] << " [" << sourceMap[source] << "]: "<< message << std::endl;
}

WindowLinux::WindowLinux(int width, int height):
        RenderTarget(width, height) {
    this->createWindow("OpenGL Window");
    this->createContext();
}

WindowLinux::~WindowLinux() {
    this->destroyContext();
    this->destroyWindow();
}

const KeyboardState& WindowLinux::getKeyboardState() const {
    return this->keyboardState;
}

const MouseState& WindowLinux::getMouseState() const {
    return this->mouseState;
}

const MousePosition& WindowLinux::getMousePosition() const {
    return this->mousePosition;
}

bool WindowLinux::isMouseCaptured() const {
    return this->mouseCaptured;
}

void WindowLinux::captureMouse(bool capture) {
    this->mouseCaptured = capture;

    if (this->mouseCaptured) {
        char data[8] = { };
        Pixmap pixmap = XCreateBitmapFromData(this->display, this->window, data, 1, 1);

        XColor color = { };
        Cursor cursor = XCreatePixmapCursor(this->display, pixmap, pixmap, &color, &color, 0, 0);

        XFreePixmap(this->display, pixmap);
        XDefineCursor(this->display, this->window, cursor);

        XWarpPointer(this->display, None, this->window, 0, 0, 0, 0, (this->width >> 1), (this->height >> 1));
    } else {
        XUndefineCursor(this->display, this->window);
    }
}

void WindowLinux::update() {
    RenderTarget::update();
    glXSwapBuffers(this->display, this->window);
}

void WindowLinux::dispatchEvents() {
    XEvent event;

    while (XPending(this->display)) {
        XNextEvent(this->display, &event);

        switch (event.type) {
            case KeyPress:
            case KeyRelease: {
                bool keyState = (event.type == KeyPress);
                this->setKeyboardState(event.xkey.keycode, keyState);
                this->onKeyboardButton(event.xkey.keycode, keyState);
                break;
            }

            case ButtonPress:
            case ButtonRelease: {
                MouseButton mouseButton = static_cast<MouseButton>(event.xbutton.button);
                bool mouseButtonState = (event.type == ButtonPress);
                this->setMouseState(mouseButton, mouseButtonState);
                this->onMouseButton(mouseButton, mouseButtonState);
                break;
            }

            case MotionNotify: {
                int xPosition = event.xmotion.x;
                int yPosition = event.xmotion.y;
                bool mouseMoved = true;

                if (this->isMouseCaptured()) {
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
                    this->setMousePosition(xPosition, yPosition);
                    this->onMouseMotion(xPosition, yPosition);
                }

                break;
            }

            case ClientMessage:
                if (event.xclient.data.l[0] == (long)this->wmDeleteWindow) {
                    this->onQuit();
                }

                break;

            default:
                break;
        }
    }

    this->onIdle();
}

void WindowLinux::setKeyboardState(int scancode, bool state) {
    this->keyboardState[scancode] = state;
}

void WindowLinux::setMouseState(MouseButton button, bool state) {
    this->mouseState[button] = state;
}

void WindowLinux::setMousePosition(int x, int y) {
    this->mousePosition.first = x;
    this->mousePosition.second = y;
}

void WindowLinux::createWindow(const char* windowName) {
    this->display = XOpenDisplay(nullptr);
    if (this->display == nullptr) {
        throw std::runtime_error("XOpenDisplay()");
    }

    int screen = XDefaultScreen(this->display);
    Window rootWindow = XRootWindow(this->display, screen);

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
        throw std::runtime_error("glXChooseFBConfig()");
    }

    this->fbConfig = fbConfigs[0];
    XFree(fbConfigs);

    XVisualInfo* visualInfo = glXGetVisualFromFBConfig(this->display, this->fbConfig);
    this->colormap = XCreateColormap(this->display, rootWindow, visualInfo->visual, AllocNone);
    if (this->colormap == None) {
        throw std::runtime_error("XCreateColormap()");
    }

    XSetWindowAttributes windowAttributes = { };
    windowAttributes.colormap = this->colormap;
    windowAttributes.event_mask = PointerMotionMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;

    this->window = XCreateWindow(
            this->display, rootWindow, 0, 0, this->width, this->height, 0,
            visualInfo->depth, InputOutput, visualInfo->visual, CWColormap | CWEventMask, &windowAttributes);
    if (this->window == None) {
        throw std::runtime_error("XCreateWindow()");
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
        throw std::runtime_error("glXCreateContextAttribsARB()");
    }

    if (!glXIsDirect(this->display, this->renderingContext)) {
        throw std::runtime_error("glXIsDirect()");
    }

    if (!glXMakeCurrent(this->display, this->window, this->renderingContext)) {
        throw std::runtime_error("glXMakeCurrent()");
    }

    OpenGL::loadCore();
    OpenGL::loadExtensions();

    std::cout
            << "Vendor: " << glGetString(GL_VENDOR) << std::endl
            << "Renderer: " << glGetString(GL_RENDERER) << std::endl
            << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl
            << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    if (OpenGL::isExtensionSupported("GL_ARB_debug_output")) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
        glDebugMessageCallbackARB(debugHandler, nullptr);
    } else {
        std::cout << "GL_ARB_debug_output unavailable, OpenGL debug disabled" << std::endl;
    }

    glFrontFace(GL_CW);
    glBlendFunc(GL_ONE, GL_ONE);
    glEnable(GL_CULL_FACE);
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
