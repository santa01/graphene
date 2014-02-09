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

#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <Camera.h>
#include <Light.h>
#include <Entity.h>
#include <Mesh.h>
#include <ImageTexture.h>
#include <Shader.h>
#include <unordered_map>
#include <memory>

namespace Graphene {

class ObjectManager: public NonCopyable {
public:
    std::shared_ptr<Camera> createCamera() const {
        return std::make_shared<Camera>();
    }

    std::shared_ptr<Light> createLight() const {
        return std::make_shared<Light>();
    }

    std::shared_ptr<Entity> createEntity(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<ImageTexture>> textureCache;
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaderCache;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshCache;
};

}  // namespace Graphene

#endif  // OBJECTMANAGER_H
