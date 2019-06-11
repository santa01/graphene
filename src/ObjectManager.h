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

#include <GrapheneApi.h>
#include <Camera.h>
#include <Light.h>
#include <Entity.h>
#include <Mesh.h>
#include <ImageTexture.h>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>

namespace Graphene {

#pragma pack(push, 1)

typedef struct {
    char magic[4];
    char major;
    char minor;
    char patch;
    char objects;
} EntityHeader;

typedef struct {
    int vertices;
    int faces;
} ObjectGeometry;

typedef struct {
    float ambientIntensity;
    float diffuseIntensity;
    float diffuseColor[3];
    float specularIntensity;
    int specularHardness;
    float specularColor[3];
    char diffuseTexture[256];
} ObjectMaterial;

#pragma pack(pop)

class ObjectManager: public NonCopyable {
public:
    GRAPHENE_API std::shared_ptr<Camera> createCamera() const;
    GRAPHENE_API std::shared_ptr<Light> createLight() const;
    GRAPHENE_API std::shared_ptr<Entity> createEntity(const std::string& name);

private:
    std::shared_ptr<ImageTexture> createTexture(const std::string& name);
    std::unordered_set<std::shared_ptr<Mesh>> createMeshes(const std::string& name);

    std::unordered_map<std::string, std::shared_ptr<ImageTexture>> textureCache;
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<Mesh>>> meshCache;
};

}  // namespace Graphene

#endif  // OBJECTMANAGER_H
