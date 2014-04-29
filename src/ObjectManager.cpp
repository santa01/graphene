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
#include <Mesh.h>
#include <Material.h>
#include <stdexcept>
#include <utility>
#include <fstream>
#include <cstring>

namespace Graphene {

std::shared_ptr<Entity> ObjectManager::createEntity(const std::string &name) {
    if (this->entityCache.find(name) != this->entityCache.end()) {
        return this->entityCache.at(name);
    }

    std::ifstream file(name, std::ios::binary);
    if (!file.good()) {
        throw std::runtime_error("Failed to open `" + name + "'");
    }

    EntityHeader entityHeader;
    ObjectGeometry objectGeometry;
    ObjectMaterial objectMaterial;

    file.read(reinterpret_cast<char*>(&entityHeader), sizeof(entityHeader));
    if (memcmp(entityHeader.magic, "GPHN", 4) != 0) {
        throw std::runtime_error("Invalid magic number");
    }

    auto entity = std::make_shared<Entity>();
    for (int i = 0; i < entityHeader.objects; i++) {
        auto material = std::make_shared<Material>();

        file.read(reinterpret_cast<char*>(&objectMaterial), sizeof(objectMaterial));
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
        entity->addMesh(mesh);
    }

    this->entityCache.insert(std::make_pair(name, entity));
    return entity;
}

}  // namespace Graphene
