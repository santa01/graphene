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
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>

namespace Graphene {

class Window: public RenderTarget {
public:
    Window(int width, int height);

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

    void dispatchEvents();

    Signals::Signal<const SDL_Event*> onMouseEvent;  // Mouse event
    Signals::Signal<const SDL_Event*> onKeyEvent;    // Keyboard event
    Signals::Signal<const SDL_Event*> onQuitEvent;   // User-requested quit

private:
    SDL_Window* window;
    SDL_GLContext context;
};

}  // namespace Graphene

#endif  // WINDOW_H
