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

#include <ObjectManager.h>
#include <EngineConfig.h>
#include <Logger.h>
#include <Material.h>
#include <TgaImage.h>
#include <Vec3.h>
#include <stdexcept>
#include <utility>
#include <fstream>

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
    float ambientIntensity;
    float diffuseIntensity;
    float diffuseColor[3];
    float specularIntensity;
    int specularHardness;
    float specularColor[3];
    char diffuseTexture[256];
} ObjectMaterial;

typedef struct {
    int vertices;
    int faces;
} ObjectGeometry;

typedef struct {
    float vertices[12] = { -1.0f, -1.0f,  0.0f, -1.0f,  1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  1.0f, -1.0f,  0.0f };
    float normals[12]  = {  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f };
    float uvs[8]       = {  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f };
    int faces[6]       = { 0, 1, 3, 1, 2, 3 };  // Clockwise face winding
} QuadMesh;

#pragma pack(pop)

ObjectManager& ObjectManager::getInstance() {
    static ObjectManager instance;
    return instance;
}

std::shared_ptr<Camera> ObjectManager::createCamera(ProjectionType type) const {
    auto camera = std::make_shared<Camera>(type);
    camera->setFov(GetEngineConfig().getFov());

    return camera;
}

std::shared_ptr<Light> ObjectManager::createLight(LightType type) const {
    return std::make_shared<Light>(type);
}

std::shared_ptr<Entity> ObjectManager::createEntity(const std::string& name) {
    LogDebug("Create entity from '%s'", name.c_str());
    std::unordered_set<std::shared_ptr<Mesh>> meshes;

    if (this->meshCache.find(name) != this->meshCache.end()) {
        LogDebug("Found cached '%s' entity, use as-is", name.c_str());
        meshes = this->meshCache.at(name);
    } else {
        meshes = this->createMeshes(name);
        this->meshCache.insert(std::make_pair(name, meshes));
    }

    auto entity = std::make_shared<Entity>();
    for (auto& mesh: meshes) {
        entity->addMesh(mesh);
    }

    return entity;
}

std::shared_ptr<Mesh> ObjectManager::createQuad() {
    std::string name(typeid(QuadMesh).name());

    if (this->meshCache.find(name) == this->meshCache.end()) {
        QuadMesh meshData;
        auto quadMesh = std::make_shared<Mesh>(&meshData, 4, 2);
        std::unordered_set<std::shared_ptr<Mesh>> meshes = { quadMesh };

        this->meshCache.insert(std::make_pair(name, meshes));
    }

    return *this->meshCache.at(name).begin();
}

void ObjectManager::clearCache() {
    LogDebug("Clear %d cached textures", this->textureCache.size());
    this->textureCache.clear();

    LogDebug("Clear %d cached meshes", this->meshCache.size());
    this->meshCache.clear();
}

std::shared_ptr<ImageTexture> ObjectManager::createTexture(const std::string& name) {
    LogDebug("Create texture from '%s'", name.c_str());

    if (this->textureCache.find(name) != this->textureCache.end()) {
        LogDebug("Found cached '%s' texture, use as-is", name.c_str());
        return this->textureCache.at(name);
    }

    TgaImage image(name);
    auto texture = std::make_shared<ImageTexture>(image);
    this->textureCache.insert(std::make_pair(name, texture));
    return texture;
}

std::unordered_set<std::shared_ptr<Mesh>> ObjectManager::createMeshes(const std::string& name) {
    std::ifstream file(name, std::ios::binary);
    if (!file.good()) {
        throw std::runtime_error(LogFormat("Failed to open '%s'", name.c_str()));
    }

    EntityHeader entityHeader;
    file.read(reinterpret_cast<char*>(&entityHeader), sizeof(entityHeader));

    std::string magic(entityHeader.magic, 4);
    if (magic != "GPHN") {
        throw std::runtime_error(LogFormat("Invalid magic number"));
    }

    LogDebug("Create %d meshes from '%s'", entityHeader.objects, name.c_str());

    ObjectMaterial objectMaterial;
    ObjectGeometry objectGeometry;
    std::unordered_set<std::shared_ptr<Mesh>> meshes;

    for (int i = 0; i < entityHeader.objects; i++) {
        auto material = std::make_shared<Material>();

        file.read(reinterpret_cast<char*>(&objectMaterial), sizeof(objectMaterial));
        material->setAmbientIntensity(objectMaterial.ambientIntensity);
        material->setDiffuseIntensity(objectMaterial.diffuseIntensity);
        material->setDiffuseColor(Math::Vec3(objectMaterial.diffuseColor[0],
                                             objectMaterial.diffuseColor[1],
                                             objectMaterial.diffuseColor[2]));

        material->setSpecularIntensity(objectMaterial.specularIntensity);
        material->setSpecularHardness(objectMaterial.specularHardness);
        material->setSpecularColor(Math::Vec3(objectMaterial.specularColor[0],
                                              objectMaterial.specularColor[1],
                                              objectMaterial.specularColor[2]));

        std::string diffuseTexture(objectMaterial.diffuseTexture);
        if (!diffuseTexture.empty()) {
            std::string parentDirectory(name.substr(0, name.find_last_of('/') + 1));
            material->setDiffuseTexture(this->createTexture(parentDirectory + diffuseTexture));
        }

        file.read(reinterpret_cast<char*>(&objectGeometry), sizeof(objectGeometry));
        int meshDataSize = sizeof(float) * objectGeometry.vertices * (3 + 3 + 2) +
                           sizeof(int) * objectGeometry.faces * 3;

        std::unique_ptr<char[]> meshData(new char[meshDataSize]);
        file.read(meshData.get(), meshDataSize);

        auto mesh = std::make_shared<Mesh>(meshData.get(), objectGeometry.vertices, objectGeometry.faces);
        mesh->setMaterial(material);
        meshes.insert(mesh);
    }

    return meshes;
}

}  // namespace Graphene
