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

#include <Engine.h>
#include <chrono>

namespace Graphene {

Engine::Engine(int width, int height):
        window(new Window(width, height)) {
    this->sceneManagers.insert(std::make_shared<SceneManager>());
    this->objectManagers.insert(std::make_shared<ObjectManager>());

    this->window->onMouseMotion.connect(std::bind(&Engine::onMouseMotion,
            this, std::placeholders::_1, std::placeholders::_2));
    this->window->onMouseButton.connect(std::bind(&Engine::onMouseButton,
            this, std::placeholders::_1, std::placeholders::_2));
    this->window->onKeyboardButton.connect(std::bind(&Engine::onKeyboardButton,
            this, std::placeholders::_1, std::placeholders::_2));

    this->window->onQuit.connect(std::bind(&Engine::onQuit, this));
    this->window->onIdle.connect(std::bind(&Engine::onIdle, this));

    this->frameTime = 0.0f;
    this->running = true;
    this->result = 0;
}

int Engine::exec() {
    std::chrono::time_point<std::chrono::steady_clock> timestamp;

    while (this->running) {
        timestamp = std::chrono::steady_clock::now();
        this->window->dispatchEvents();

        for (auto& frameBuffer: this->frameBuffers) {
            if (frameBuffer->isAutoUpdate()) {
                frameBuffer->update();
            }
        }
        this->window->update();

        std::chrono::duration<float> duration = std::chrono::steady_clock::now() - timestamp;
        this->frameTime = duration.count();
    }

    return this->result;
}

}  // namespace Graphene
