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

#include <GrapheneApi.h>
#include <NonCopyable.h>
#include <FrameBuffer.h>
#include <Window.h>
#include <Scene.h>
#include <Input.h>
#include <OpenGL.h>
#include <TextComponent.h>
#include <Signals.h>
#include <vector>
#include <memory>
#include <string>

namespace Graphene {

class Engine: public NonCopyable {
public:
    GRAPHENE_API Engine();
    GRAPHENE_API virtual ~Engine() = default;

    GRAPHENE_API const std::vector<std::shared_ptr<FrameBuffer>>& getFrameBuffers() const;
    GRAPHENE_API const std::shared_ptr<FrameBuffer>& createFrameBuffer(int width, int height, GLenum format);

    GRAPHENE_API const std::vector<std::shared_ptr<Scene>>& getScenes() const;
    GRAPHENE_API const std::shared_ptr<Scene>& createScene(const std::string& name);
    GRAPHENE_API const std::shared_ptr<Scene>& createScene();

    GRAPHENE_API const std::shared_ptr<Window>& getWindow() const;

    GRAPHENE_API unsigned int getFrame() const;
    GRAPHENE_API float getFrameTime() const;

    GRAPHENE_API void exit(int result);
    GRAPHENE_API int exec();

protected:
    virtual void onMouseMotion(int /*x*/, int /*y*/) { }
    virtual void onMouseButton(MouseButton /*button*/, bool /*state*/) { }
    virtual void onKeyboardKey(KeyboardKey /*key*/, bool /*state*/) { }
    virtual void onSetup() { }
    virtual void onTeardown() { }
    virtual void onIdle() { }
    virtual void onQuit() { }

private:
    void setupWindow();
    void setupOpenGL();
    void setupEngine();
    void teardownEngine();
    void update();

    void onSetupDebug();
    void onTeardownDebug();
    void onIdleDebug();

    Signals::Signal<> onSetupSignal;     // Once before event loop
    Signals::Signal<> onTeardownSignal;  // Once after event loop
    Signals::Signal<> onIdleSignal;      // Empty event queue
    Signals::Signal<> onQuitSignal;      // User-requested quit

    std::vector<std::shared_ptr<FrameBuffer>> frameBuffers;
    std::vector<std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Window> window;

    std::shared_ptr<TextComponent> fpsDebug;  // Debug overlay

    unsigned int frame = 0;
    float frameTime = 0.0f;

    bool running = true;
    int result = 0;
};

}  // namespace Graphene

#endif  // ENGINE_H
