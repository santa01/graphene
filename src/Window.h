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

#include <RenderTarget.h>
#include <Signals.h>
#include <utility>
#include <algorithm>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <iostream>

namespace Graphene {

class Graphene;

class Window: public RenderTarget {
public:
    ~Window() {
        if (this->context) {
            SDL_GL_DeleteContext(this->context);
        }

        if (this->window) {
            SDL_DestroyWindow(this->window);
        }

        IMG_Quit();
        SDL_Quit();
    }

    std::vector<bool> getKeyboardState() const {
        int keysCount;
        const Uint8* state = SDL_GetKeyboardState(&keysCount);

        std::vector<bool> keyboardState;
        std::copy(state, state + keysCount, std::back_inserter(keyboardState));

        return keyboardState;  // RVO on assignment
    }

    std::vector<bool> getMouseState() const {
        Uint32 state = SDL_GetMouseState(nullptr, nullptr);

        std::vector<bool> mouseState(SDL_BUTTON_X2);
        std::generate(mouseState.begin(), mouseState.end(), [state]() {
            static int button = 1;
            return state & SDL_BUTTON(button++);
        });

        return mouseState;
    }

    std::pair<int, int> getMousePosition() const {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return std::make_pair(x, y);
    }

    void captureMouse(bool capture) {
        SDL_WarpMouseInWindow(this->window, this->width / 2, this->height / 2);
        SDL_SetRelativeMouseMode(capture ? SDL_TRUE : SDL_FALSE);
    }

    void update() {
        RenderTarget::update();
        SDL_GL_SwapWindow(this->window);
    }

    void dispatchEvents();

private:
    friend class Engine;
    Window(int width, int height);

    Signals::Signal<int, int> onMouseMotion;
    Signals::Signal<int, bool> onMouseButton;
    Signals::Signal<int, bool> onKeyboardButton;
    Signals::Signal<> onQuit;  // User-requested quit
    Signals::Signal<> onIdle;  // Empty event queue

    SDL_Window* window;
    SDL_GLContext context;
};

}  // namespace Graphene

#endif  // WINDOW_H
