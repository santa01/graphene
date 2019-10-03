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
    float vertices[18] = {  1.0f, -1.0f,  0.0f, -1.0f, -1.0f,  0.0f, -1.0f,  1.0f, -0.0f,
                            1.0f, -1.0f,  0.0f, -1.0f,  1.0f, -0.0f,  1.0f,  1.0f, -0.0f };
    float normals[18]  = {  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                            0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f };
    float uvs[12]      = {  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                            1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f };
    int faces[6]       = {  0,  1,  2,
                            3,  4,  5 };  // Clockwise face winding
} QuadMesh;

typedef struct {
    float vertices[108] = {  1.0, -1.0, -1.0,  1.0, -1.0,  1.0, -1.0, -1.0,  1.0,
                            -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0,
                             1.0,  1.0,  1.0,  1.0, -1.0,  1.0,  1.0, -1.0, -1.0,
                             1.0,  1.0, -1.0,  1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
                            -1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0,
                             1.0, -1.0,  1.0,  1.0,  1.0,  1.0, -1.0,  1.0,  1.0,
                             1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0, -1.0,
                            -1.0,  1.0,  1.0,  1.0,  1.0, -1.0, -1.0,  1.0, -1.0,
                             1.0,  1.0,  1.0,  1.0, -1.0, -1.0,  1.0,  1.0, -1.0,
                             1.0,  1.0, -1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0,
                            -1.0, -1.0, -1.0, -1.0,  1.0,  1.0, -1.0,  1.0, -1.0,
                             1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0, -1.0,  1.0f };
    float normals[108]  = {  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,
                             0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,
                             1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,
                             0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,
                            -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0,
                             0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,
                             0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,
                             0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,
                             1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,
                             0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,
                            -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0,
                             0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0f };
    float uvs[72]       = {  0.25, 0.50, 0.25, 0.75, 0.50, 0.75,
                             0.50, 0.00, 0.25, 0.00, 0.25, 0.25,
                             0.00, 0.75, 0.25, 0.75, 0.25, 0.50,
                             0.25, 0.25, 0.25, 0.50, 0.50, 0.50,
                             0.50, 0.50, 0.50, 0.75, 0.75, 0.75,
                             0.25, 0.75, 0.25, 1.00, 0.50, 1.00,
                             0.25, 0.50, 0.50, 0.75, 0.50, 0.50,
                             0.50, 0.00, 0.25, 0.25, 0.50, 0.25,
                             0.00, 0.75, 0.25, 0.50, 0.00, 0.50,
                             0.25, 0.25, 0.50, 0.50, 0.50, 0.25,
                             0.50, 0.50, 0.75, 0.75, 0.75, 0.50,
                             0.25, 0.75, 0.50, 1.00, 0.50, 0.75f };
    int faces[36]       = {  0,  1,  2,
                             3,  4,  5,
                             6,  7,  8,
                             9, 10, 11,
                            12, 13, 14,
                            15, 16, 17,
                            18, 19, 20,
                            21, 22, 23,
                            24, 25, 26,
                            27, 28, 29,
                            30, 31, 32,
                            33, 34, 35 };  // Clockwise face winding
} CubeMesh;

#pragma pack(pop)

template<typename T>
std::shared_ptr<Mesh> geometryToMesh(bool flipGeometry) {
    T geometry;
    int vertexElements = sizeof(geometry.vertices) / sizeof(float);
    int vertexIndices = sizeof(geometry.faces) / sizeof(int);

    if (flipGeometry) {
        // Flip normals
        for (int i = 0; i < vertexElements; i += 3) {
            geometry.normals[i + 0] = -geometry.normals[i + 0];
            geometry.normals[i + 1] = -geometry.normals[i + 1];
            geometry.normals[i + 2] = -geometry.normals[i + 2];
        }
        // Flip face winding
        for (int i = 0; i < vertexIndices; i += 3) {
            std::swap(geometry.faces[i + 0], geometry.faces[i + 2]);
        }
    }

    return std::make_shared<Mesh>(&geometry, vertexElements / 3, vertexIndices / 3);
}

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

std::shared_ptr<Skybox> ObjectManager::createSkybox(const std::string& name) {
    return std::make_shared<Skybox>(this->loadCubeTexture(name));
}

std::shared_ptr<Entity> ObjectManager::createEntity(const std::string& name) {
    std::vector<std::shared_ptr<Mesh>> meshes;

    if (this->meshCache.find(name) != this->meshCache.end()) {
        LogDebug("Reuse cached '%s' entity", name.c_str());
        meshes = this->meshCache.at(name);
    } else {
        LogDebug("Load entity from '%s'", name.c_str());
        meshes = this->loadMeshes(name);
        this->meshCache.emplace(name, meshes);
    }

    auto entity = std::make_shared<Entity>();
    for (auto& mesh: meshes) {
        entity->addMesh(mesh);
    }

    return entity;
}

std::shared_ptr<Shader> ObjectManager::createShader(const std::string& name) {
    if (this->shaderCache.find(name) != this->shaderCache.end()) {
        LogDebug("Reuse cached '%s' shader", name.c_str());
        return this->shaderCache.at(name);
    }

    LogDebug("Load shader from '%s'", name.c_str());
    auto shader = std::make_shared<Shader>(GetEngineConfig().getDataDirectory() + '/' + name);

    this->shaderCache.emplace(name, shader);
    return shader;
}

std::shared_ptr<Mesh> ObjectManager::createQuad(MeshWinding winding) {
    return this->createMesh("QuadMesh", winding, [winding]() {
        return geometryToMesh<QuadMesh>(winding == MeshWinding::WINDING_COUNTER_CLOCKWISE);
    });
}

std::shared_ptr<Mesh> ObjectManager::createCube(MeshWinding winding) {
    return this->createMesh("CubeMesh", winding, [winding]() {
        return geometryToMesh<CubeMesh>(winding == MeshWinding::WINDING_COUNTER_CLOCKWISE);
    });
}

void ObjectManager::clearCache() {
    LogDebug("Clear shader cache (%d items)", this->shaderCache.size());
    this->shaderCache.clear();

    LogDebug("Clear texture cache (%d items)", this->textureCache.size());
    this->textureCache.clear();

    LogDebug("Clear mesh cache (%d items)", this->meshCache.size());
    this->meshCache.clear();
}

std::shared_ptr<ImageTexture> ObjectManager::loadTexture(const std::string& name) {
    if (this->textureCache.find(name) != this->textureCache.end()) {
        LogDebug("Reuse cached '%s' texture", name.c_str());
        return std::dynamic_pointer_cast<ImageTexture>(this->textureCache.at(name));
    }

    LogDebug("Load texture from '%s'", name.c_str());
    TgaImage textureImage(GetEngineConfig().getDataDirectory() + '/' + name, true);
    auto texture = std::make_shared<ImageTexture>(textureImage);

    this->textureCache.emplace(name, texture);
    return texture;
}

std::shared_ptr<ImageCubeTexture> ObjectManager::loadCubeTexture(const std::string& name) {
    if (this->textureCache.find(name) != this->textureCache.end()) {
        LogDebug("Reuse cached '%s/*.tga' textures", name.c_str());
        return std::dynamic_pointer_cast<ImageCubeTexture>(this->textureCache.at(name));
    }

    LogDebug("Load textures from '%s/*.tga'", name.c_str());

    std::string textureRoot(GetEngineConfig().getDataDirectory() + '/' + name);
    CubeImage textureCubeImage = {
        std::make_shared<TgaImage>(textureRoot + "/positive_x.tga", false),
        std::make_shared<TgaImage>(textureRoot + "/negative_x.tga", false),
        std::make_shared<TgaImage>(textureRoot + "/positive_y.tga", false),
        std::make_shared<TgaImage>(textureRoot + "/negative_y.tga", false),
        std::make_shared<TgaImage>(textureRoot + "/positive_z.tga", false),
        std::make_shared<TgaImage>(textureRoot + "/negative_z.tga", false)
    };

    auto texture = std::make_shared<ImageCubeTexture>(textureCubeImage);
    this->textureCache.emplace(name, texture);
    return texture;
}

std::vector<std::shared_ptr<Mesh>> ObjectManager::loadMeshes(const std::string& name) {
    std::ifstream file(GetEngineConfig().getDataDirectory() + '/' + name, std::ios::binary);
    if (!file) {
        throw std::runtime_error(LogFormat("Failed to open '%s'", name.c_str()));
    }

    EntityHeader entityHeader;
    file.read(reinterpret_cast<char*>(&entityHeader), sizeof(entityHeader));

    std::string magic(entityHeader.magic, 4);
    if (magic != "GPHN") {
        throw std::runtime_error(LogFormat("Invalid magic number"));
    }

    LogDebug("Load %d meshes from '%s'", entityHeader.objects, name.c_str());

    ObjectMaterial objectMaterial;
    ObjectGeometry objectGeometry;
    std::vector<std::shared_ptr<Mesh>> meshes;

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
            material->setDiffuseTexture(this->loadTexture(parentDirectory + diffuseTexture));
        }

        file.read(reinterpret_cast<char*>(&objectGeometry), sizeof(objectGeometry));
        int meshDataSize = sizeof(float) * objectGeometry.vertices * (3 + 3 + 2) +
                           sizeof(int) * objectGeometry.faces * 3;

        std::unique_ptr<char[]> meshData(new char[meshDataSize]);
        file.read(meshData.get(), meshDataSize);

        auto mesh = std::make_shared<Mesh>(meshData.get(), objectGeometry.vertices, objectGeometry.faces);
        mesh->setMaterial(material);

        meshes.push_back(mesh);
    }

    return meshes;
}

std::shared_ptr<Mesh> ObjectManager::createMesh(const std::string& alias, MeshWinding winding, MeshFactory factory) {
    if (this->meshCache.find(alias) == this->meshCache.end()) {
        std::vector<std::shared_ptr<Mesh>> meshes = { nullptr, nullptr };
        this->meshCache.emplace(alias, meshes);
    }

    auto& meshes = this->meshCache.at(alias);
    if (meshes.at(winding) == nullptr) {
        meshes.at(winding) = factory();
    }

    // Return a Mesh copy (same OpenGL objects, different material)
    auto mesh = std::make_shared<Mesh>(*meshes.at(winding));
    mesh->setMaterial(std::make_shared<Material>());

    return mesh;
}

}  // namespace Graphene
