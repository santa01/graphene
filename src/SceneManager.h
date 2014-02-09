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

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <NonCopyable.h>
#include <SceneNode.h>
#include <Camera.h>
#include <memory>

namespace Graphene {

class SceneManager: public std::enable_shared_from_this<SceneManager>, public NonCopyable {
public:
    SceneManager() {
        this->lightPass = false;
        this->shadowPass = false;

        this->rootNode = std::make_shared<SceneNode>(this->shared_from_this());
    }

    std::shared_ptr<SceneNode> getRootNode() {
        return this->rootNode;
    }

    void setLightPass(bool lightPass) {
        this->lightPass = lightPass;
    }

    bool isLightPass() const {
        return this->lightPass;
    }

    void setShadowPass(bool shadowPass) {
        this->shadowPass = shadowPass;
    }

    bool isShadowPass() const {
        return this->shadowPass;
    }

    void render(std::shared_ptr<Camera> camera) {
        // TODO
    }

private:
    std::shared_ptr<SceneNode> rootNode;

    bool lightPass;
    bool shadowPass;
};

}  // namespace Graphene

#endif  // SCENEMANAGER_H
