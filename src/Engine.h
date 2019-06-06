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

#ifndef ENGINE_H
#define ENGINE_H

#include <NonCopyable.h>
#include <FrameBuffer.h>
#if defined(_WIN32)
#include <WindowWin32.h>
typedef Graphene::WindowWin32 Window;
#elif defined(__linux__)
#include <WindowLinux.h>
typedef Graphene::WindowLinux Window;
#endif
#include <SceneManager.h>
#include <ObjectManager.h>
#include <unordered_set>
#include <stdexcept>
#include <memory>

namespace Graphene {

class Engine: public NonCopyable {
public:
    Engine(int width, int height);
    virtual ~Engine() {}

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

    float getFrameTime() const {
        return this->frameTime;
    }

    void exit(int result) {
        this->result = result;
        this->running = false;
    }

    int exec();

protected:
    virtual void onMouseMotion(int /*x*/, int /*y*/) {}
    virtual void onMouseButton(int /*button*/, bool /*state*/) {}
    virtual void onKeyboardButton(int /*button*/, bool /*state*/) {}
    virtual void onQuit() { this->exit(0); }
    virtual void onIdle() {}

private:
    std::unordered_set<std::shared_ptr<FrameBuffer>> frameBuffers;
    std::unordered_set<std::shared_ptr<SceneManager>> sceneManagers;
    std::unordered_set<std::shared_ptr<ObjectManager>> objectManagers;
    std::shared_ptr<Window> window;

    float frameTime;
    bool running;
    int result;
};

}  // namespace Graphene

#endif  // ENGINE_H
