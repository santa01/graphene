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

#ifndef GRAPHENE_H
#define GRAPHENE_H

#include <NonCopyable.h>
#include <Window.h>
#include <FrameBuffer.h>
#include <SDL2/SDL_events.h>
#include <unordered_set>
#include <memory>
#include <functional>
#include <stdexcept>

namespace Graphene {

class Graphene: public NonCopyable {
public:
    Graphene(int width, int height):
            window(new Window(width, height)) {
    }

    const std::unordered_set<std::shared_ptr<FrameBuffer>>& getFrameBuffers() const {
        return this->frameBuffers;
    }

    void addFrameBuffer(const std::shared_ptr<FrameBuffer> frameBuffer) {
        if (frameBuffer == nullptr) {
            throw std::invalid_argument("FrameBuffer cannot be nullptr");
        }

        this->frameBuffers.insert(frameBuffer);
    }

    const std::shared_ptr<Window> getWindow() const {
        return this->window;
    }

    void setMouseEventHandler(const std::function<void(const SDL_Event*)>& handler) {
        this->window->onMouseEvent.connect(handler);
    }

    void setKeyEventHandler(const std::function<void(const SDL_Event*)>& handler) {
        this->window->onKeyEvent.connect(handler);
    }

    void setQuitEventHandler(const std::function<void(const SDL_Event*)>& handler) {
        this->window->onQuitEvent.connect(handler);
    }

    void dispatchEvents() {
        this->window->dispatchEvents();
    }

    void render() {
        for (auto& frameBuffer: this->frameBuffers) {
            if (frameBuffer->isAutoUpdate()) {
                frameBuffer->update();
            }
        }

        this->window->update();
    }

private:
    std::unordered_set<std::shared_ptr<FrameBuffer>> frameBuffers;
    std::shared_ptr<Window> window;
};

}  // namespace Graphene

#endif  // GRAPHENE_H
