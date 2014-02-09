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
#include <FrameBuffer.h>
#include <Window.h>
#include <SceneManager.h>
#include <ObjectManager.h>
#include <SDL2/SDL_events.h>
#include <unordered_set>
#include <stdexcept>
#include <memory>

namespace Graphene {

class Graphene: public NonCopyable {
public:
    Graphene(int width, int height):
            window(new Window(width, height)) {
        this->sceneManagers.insert(std::make_shared<SceneManager>());
        this->objectManagers.insert(std::make_shared<ObjectManager>());
    }

    const std::unordered_set<std::shared_ptr<FrameBuffer>>& getFrameBuffers() {
        return this->frameBuffers;
    }

    void addFrameBuffer(const std::shared_ptr<FrameBuffer> frameBuffer) {
        if (frameBuffer == nullptr) {
            throw std::invalid_argument("FrameBuffer cannot be nullptr");
        }

        this->frameBuffers.insert(frameBuffer);
    }

    const std::unordered_set<std::shared_ptr<SceneManager>>& getSceneManagers() {
        return this->sceneManagers;
    }

    void addSceneManager(const std::shared_ptr<SceneManager> sceneManager) {
        if (sceneManager == nullptr) {
            throw std::invalid_argument("SceneManager cannot be nullptr");
        }

        this->sceneManagers.insert(sceneManager);
    }

    const std::unordered_set<std::shared_ptr<ObjectManager>>& getObjectManagers() {
        return this->objectManagers;
    }

    void addObjectManager(const std::shared_ptr<ObjectManager> objectManager) {
        if (objectManager == nullptr) {
            throw std::invalid_argument("ObjectManager cannot be nullptr");
        }

        this->objectManagers.insert(objectManager);
    }

    std::shared_ptr<Window> getWindow() {
        return this->window;
    }

    void update() {
        this->window->dispatchEvents();

        for (auto& frameBuffer: this->frameBuffers) {
            if (frameBuffer->isAutoUpdate()) {
                frameBuffer->update();
            }
        }

        this->window->update();
    }

private:
    std::unordered_set<std::shared_ptr<FrameBuffer>> frameBuffers;
    std::unordered_set<std::shared_ptr<SceneManager>> sceneManagers;
    std::unordered_set<std::shared_ptr<ObjectManager>> objectManagers;

    std::shared_ptr<Window> window;
};

}  // namespace Graphene

#endif  // GRAPHENE_H
