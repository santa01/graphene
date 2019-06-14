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
#include <Logger.h>
#include <Material.h>
#include <Vec3.h>
#include <stdexcept>
#include <utility>
#include <fstream>

namespace Graphene {

std::shared_ptr<Camera> ObjectManager::createCamera() const {
    return std::make_shared<Camera>();
}

std::shared_ptr<Light> ObjectManager::createLight() const {
    return std::make_shared<Light>();
}

std::shared_ptr<Entity> ObjectManager::createEntity(const std::string& name) {
    std::unordered_set<std::shared_ptr<Mesh>> meshes;

    if (this->meshCache.find(name) != this->meshCache.end()) {
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

std::shared_ptr<ImageTexture> ObjectManager::createTexture(const std::string& name) {
    if (this->textureCache.find(name) != this->textureCache.end()) {
        return this->textureCache.at(name);
    }

    auto texture = std::make_shared<ImageTexture>(name);
    this->textureCache.insert(std::make_pair(name, texture));
    return texture;
}

std::unordered_set<std::shared_ptr<Mesh>> ObjectManager::createMeshes(const std::string& name) {
    std::ifstream file(name, std::ios::binary);
    if (!file.good()) {
        throw std::runtime_error(LogFormat("Failed to open '%s'", name));
    }

    EntityHeader entityHeader;
    file.read(reinterpret_cast<char*>(&entityHeader), sizeof(entityHeader));

    std::string magic(entityHeader.magic, 4);
    if (magic != "GPHN") {
        throw std::runtime_error(LogFormat("Invalid magic number"));
    }

    ObjectGeometry objectGeometry;
    ObjectMaterial objectMaterial;
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

        auto mesh = std::make_shared<Mesh>(meshData.get(), objectGeometry.faces, objectGeometry.vertices);
        mesh->setMaterial(material);
        meshes.insert(mesh);
    }

    return meshes;
}

}  // namespace Graphene
