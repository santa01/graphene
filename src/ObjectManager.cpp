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
#include <RawImage.h>
#include <TgaImage.h>
#include <GraphicsComponent.h>
#include <TextComponent.h>
#include <QuadComponent.h>
#include <Vec3.h>
#include <stdexcept>
#include <utility>
#include <sstream>
#include <cmath>
#include <cassert>

namespace Graphene {

#pragma pack(push, 1)

typedef struct {
    char magic[4];
    char major;
    char minor;
    char patch;
    char unused;
} GrapheneHeader;

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
    char name[256];
    float ambientColor[3];
    float ambientEnergy;
    float playerRotation[3];
    float playerPosition[3];
    char skyboxTexture[256];
    int entities;
    int lights;
} SceneDefinition;

typedef struct {
    char name[256];
    char filepath[256];
    float scaling[3];
    float rotation[3];
    float position[3];
} EntityDefinition;

typedef struct {
    char name[256];
    int type;
    float energy;
    float falloff;
    float spotAngle;
    float spotBlend;
    float color[3];
    float rotation[3];
    float position[3];
} LightDefinition;

struct QuadMesh {
    float vertices[18] = {  1.0f, -1.0f,  0.0f, -1.0f, -1.0f,  0.0f, -1.0f,  1.0f, -0.0f,
                            1.0f, -1.0f,  0.0f, -1.0f,  1.0f, -0.0f,  1.0f,  1.0f, -0.0f };
    float normals[18]  = {  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,
                            0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f };
    float uvs[12]      = {  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                            1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f };
    int faces[6]       = {  0,  1,  2,
                            3,  4,  5 };  // Clockwise face winding
};

struct CubeMesh {
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
};

#pragma pack(pop)

ObjectManager& ObjectManager::getInstance() {
    static ObjectManager instance;
    return instance;
}

const std::shared_ptr<Entity> ObjectManager::createEntity(const std::string& name) {
    auto entityIt = this->entityCache.find(name);
    if (entityIt != this->entityCache.end()) {
        LogDebug("Reuse cached '%s' entity", name.c_str());
    } else {
        LogDebug("Load entity from '%s'", name.c_str());

        std::ifstream file(GetEngineConfig().getDataDirectory() + '/' + name, std::ios::binary);
        if (!file) {
            throw std::runtime_error(LogFormat("Failed to open '%s'", name.c_str()));
        }

        this->validateHeader(file, "GPNE");

        int objectsCount;
        file.read(reinterpret_cast<char*>(&objectsCount), sizeof(objectsCount));

        LogDebug("Load %d objects from '%s'", objectsCount, name.c_str());

        ObjectMaterial objectMaterial;
        ObjectGeometry objectGeometry;

        auto& materials = this->materialCache[name];
        auto& meshes = this->meshCache[name];

        for (int i = 0; i < objectsCount; i++) {
            file.read(reinterpret_cast<char*>(&objectMaterial), sizeof(objectMaterial));

            auto material = std::make_shared<Material>();
            materials.emplace_back(material);

            material->setAmbientIntensity(objectMaterial.ambientIntensity);
            material->setDiffuseIntensity(objectMaterial.diffuseIntensity);
            material->setDiffuseColor(objectMaterial.diffuseColor[0], objectMaterial.diffuseColor[1], objectMaterial.diffuseColor[2]);

            material->setSpecularIntensity(objectMaterial.specularIntensity);
            material->setSpecularHardness(objectMaterial.specularHardness);
            material->setSpecularColor(objectMaterial.specularColor[0], objectMaterial.specularColor[1], objectMaterial.specularColor[2]);

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
            meshes.emplace_back(mesh);
        }

        this->entityCache.emplace(name);
    }

    auto& materials = this->materialCache.at(name);
    auto& meshes = this->meshCache.at(name);

    size_t materialsCount = materials.size();
    size_t meshesCount = meshes.size();
    assert(materialsCount == meshesCount);

    auto graphicsComponent = std::make_shared<GraphicsComponent>();
    for (size_t i = 0; i < materialsCount; i++) {
        graphicsComponent->addGraphics(materials.at(i), meshes.at(i));
    }

    auto entity = std::make_shared<Entity>();
    entity->addComponent(graphicsComponent);

    return entity;
}

const std::shared_ptr<Scene> ObjectManager::createScene(const std::string& name) {
    LogDebug("Load world from '%s'", name.c_str());

    std::ifstream file(GetEngineConfig().getDataDirectory() + '/' + name, std::ios::binary);
    if (!file) {
        throw std::runtime_error(LogFormat("Failed to open '%s'", name.c_str()));
    }

    this->validateHeader(file, "GPNW");

    SceneDefinition sceneDefinition;
    file.read(reinterpret_cast<char*>(&sceneDefinition), sizeof(sceneDefinition));

    auto scene = std::make_shared<Scene>();
    scene->setName(sceneDefinition.name);
    scene->setAmbientColor(sceneDefinition.ambientColor[0], sceneDefinition.ambientColor[1], sceneDefinition.ambientColor[2]);
    scene->setAmbientEnergy(sceneDefinition.ambientEnergy);

    auto& player = scene->getPlayer();
    player->translate(sceneDefinition.playerPosition[0], sceneDefinition.playerPosition[1], sceneDefinition.playerPosition[2]);
    player->rotate(Math::Vec3::UNIT_X, sceneDefinition.playerRotation[0]);
    player->rotate(Math::Vec3::UNIT_Y, sceneDefinition.playerRotation[1]);
    player->rotate(Math::Vec3::UNIT_Z, sceneDefinition.playerRotation[2]);

    std::string parentDirectory(name.substr(0, name.find_last_of('/') + 1));
    std::string skyboxTexture(sceneDefinition.skyboxTexture);
    if (!skyboxTexture.empty()) {
        scene->setSkybox(this->createSkybox(parentDirectory + skyboxTexture));
    }

    auto& sceneRoot = scene->getRoot();
    float pi = static_cast<float>(M_PI);

    for (int i = 0; i < sceneDefinition.entities; i++) {
        EntityDefinition entityDefinition;
        file.read(reinterpret_cast<char*>(&entityDefinition), sizeof(entityDefinition));

        auto entity = this->createEntity(parentDirectory + std::string(entityDefinition.filepath));
        entity->setName(entityDefinition.name);

        entity->scale(entityDefinition.scaling[0], entityDefinition.scaling[1], entityDefinition.scaling[2]);
        entity->translate(entityDefinition.position[0], entityDefinition.position[1], entityDefinition.position[2]);
        entity->rotate(Math::Vec3::UNIT_X, entityDefinition.rotation[0] * 180.0f / pi);
        entity->rotate(Math::Vec3::UNIT_Y, entityDefinition.rotation[1] * 180.0f / pi);
        entity->rotate(Math::Vec3::UNIT_Z, entityDefinition.rotation[2] * 180.0f / pi);

        sceneRoot->addObject(entity);
    }

    for (int i = 0; i < sceneDefinition.lights; i++) {
        LightDefinition lightDefinition;
        file.read(reinterpret_cast<char*>(&lightDefinition), sizeof(lightDefinition));

        auto light = this->createLight(static_cast<LightType>(lightDefinition.type));
        light->setName(lightDefinition.name);

        light->setEnergy(lightDefinition.energy);
        light->setFalloff(lightDefinition.falloff);
        light->setAngle(lightDefinition.spotAngle * 180.0f / pi);
        light->setBlend(lightDefinition.spotBlend);
        light->setColor(lightDefinition.color[0], lightDefinition.color[1], lightDefinition.color[2]);

        light->translate(lightDefinition.position[0], lightDefinition.position[1], lightDefinition.position[2]);
        light->rotate(Math::Vec3::UNIT_X, lightDefinition.rotation[0] * 180.0f / pi);
        light->rotate(Math::Vec3::UNIT_Y, lightDefinition.rotation[1] * 180.0f / pi);
        light->rotate(Math::Vec3::UNIT_Z, lightDefinition.rotation[2] * 180.0f / pi);

        sceneRoot->addObject(light);
    }

    return scene;
}

const std::shared_ptr<Camera> ObjectManager::createCamera(ProjectionType type) const {
    auto camera = std::make_shared<Camera>(type);
    camera->setFov(GetEngineConfig().getFov());

    return camera;
}

const std::shared_ptr<Light> ObjectManager::createLight(LightType type) const {
    return std::make_shared<Light>(type);
}

const std::shared_ptr<Entity> ObjectManager::createSkybox(const std::string& name) {
    auto material = std::make_shared<Material>();
    material->setDiffuseTexture(this->createCubeTexture(name));

    auto graphicsComponent = std::make_shared<GraphicsComponent>();
    graphicsComponent->addGraphics(material, this->createCube(FaceWinding::WINDING_COUNTER_CLOCKWISE));

    auto skybox = std::make_shared<Entity>();
    skybox->addComponent(graphicsComponent);

    return skybox;
}

const std::shared_ptr<Entity> ObjectManager::createLabel(int width, int height, const std::string& name, int size) {
    RawImage textureImage(width, height, 32);
    auto texture = std::make_shared<ImageTexture>(textureImage);

    auto material = std::make_shared<Material>();
    material->setDiffuseTexture(texture);

    auto graphicsComponent = std::make_shared<GraphicsComponent>();
    graphicsComponent->addGraphics(material, this->createQuad(FaceWinding::WINDING_CLOCKWISE));

    auto font = this->createFont(name, size);
    auto textComponent = std::make_shared<TextComponent>(width, height, font);

    auto quadComponent = std::make_shared<QuadComponent>(width, height);
    quadComponent->setOrigin(width / 2, height / 2);

    auto label = std::make_shared<Entity>();
    label->addComponent(graphicsComponent);
    label->addComponent(textComponent);
    label->addComponent(quadComponent);

    return label;
}

const std::shared_ptr<Mesh> ObjectManager::createQuad(FaceWinding winding) {
    return this->createMesh<QuadMesh>("QuadMesh", winding);
}

const std::shared_ptr<Mesh> ObjectManager::createCube(FaceWinding winding) {
    return this->createMesh<CubeMesh>("CubeMesh", winding);
}

const std::shared_ptr<Shader>& ObjectManager::createShader(const std::string& name) {
    auto shaderIt = this->shaderCache.find(name);
    if (shaderIt != this->shaderCache.end()) {
        LogDebug("Reuse cached '%s' shader", name.c_str());
        return shaderIt->second;
    }

    LogDebug("Load shader from '%s'", name.c_str());

    std::ifstream file(GetEngineConfig().getDataDirectory() + '/' + name, std::ios::binary);
    if (!file) {
        throw std::runtime_error(LogFormat("Failed to open '%s'", name.c_str()));
    }

    file.seekg(0, std::ios::end);
    std::ifstream::pos_type sourceLength = file.tellg();
    file.seekg(0, std::ios::beg);

    std::unique_ptr<char[]> source(new char[sourceLength]);
    file.read(source.get(), sourceLength);

    std::string shaderSource(source.get(), sourceLength);
    auto shader = std::make_shared<Shader>(shaderSource);
    shader->setName(name);

    return this->shaderCache.emplace(name, shader).first->second;
}

const std::shared_ptr<Shader>& ObjectManager::createShader() {
    static const std::string shaderName("dummy");
    static const std::string shaderSource("{SHADER_VERSION}\n{SHADER_TYPE}\nvoid main() { }\n");

    auto shaderIt = this->shaderCache.find(shaderName);
    if (shaderIt != this->shaderCache.end()) {
        return shaderIt->second;
    }

    auto shader = std::make_shared<Shader>(shaderSource);
    shader->setName(shaderName);

    return this->shaderCache.emplace(shaderName, shader).first->second;
}

const std::shared_ptr<Texture>& ObjectManager::createTexture(const std::string& name) {
    auto textureIt = this->textureCache.find(name);
    if (textureIt != this->textureCache.end()) {
        LogDebug("Reuse cached '%s' texture", name.c_str());
        return textureIt->second;
    }

    LogDebug("Load texture from '%s'", name.c_str());
    TgaImage textureImage(GetEngineConfig().getDataDirectory() + '/' + name, true);
    auto texture = std::make_shared<ImageTexture>(textureImage);

    return this->textureCache.emplace(name, texture).first->second;
}

const std::shared_ptr<Texture>& ObjectManager::createCubeTexture(const std::string& name) {
    auto textureIt = this->textureCache.find(name);
    if (textureIt != this->textureCache.end()) {
        LogDebug("Reuse cached '%s/*.tga' textures", name.c_str());
        return textureIt->second;
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
    return this->textureCache.emplace(name, texture).first->second;
}

const std::shared_ptr<Font>& ObjectManager::createFont(const std::string& name, int size) {
    std::ostringstream nameStream;
    nameStream << name << "_" << size;

    std::string nameAlias(nameStream.str());
    auto fontIt = this->fontCache.find(nameAlias);
    if (fontIt != this->fontCache.end()) {
        LogDebug("Reuse cached '%s' font (%dpt)", name.c_str(), size);
        return fontIt->second;
    }

    LogDebug("Load font (%dpt) from '%s'", size, name.c_str());
    auto font = std::make_shared<Font>(GetEngineConfig().getDataDirectory() + '/' + name, size);

    return this->fontCache.emplace(nameAlias, font).first->second;
}

void ObjectManager::teardown() {
    LogDebug("Clear shader cache (%d items)", this->shaderCache.size());
    this->shaderCache.clear();

    LogDebug("Clear texture cache (%d items)", this->textureCache.size());
    this->textureCache.clear();

    LogDebug("Clear mesh cache (%d items)", this->meshCache.size());
    this->meshCache.clear();
}

void ObjectManager::validateHeader(std::ifstream& file, const std::string& magic) {
    GrapheneHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    std::string headerMagic(header.magic, 4);
    if (headerMagic != magic) {
        throw std::runtime_error(LogFormat("Invalid magic number '%s', expected '%s'", headerMagic, magic));
    }

    std::ostringstream headerVersion;
    headerVersion << static_cast<int>(header.major)
           << "." << static_cast<int>(header.minor)
           << "." << static_cast<int>(header.patch);

    std::string grapheneVersion(GRAPHENE_VERSION);
    if (headerVersion.str() != grapheneVersion) {
        throw std::runtime_error(LogFormat("Invalid version '%s', expected '%s'", headerVersion.str(), grapheneVersion));
    }
}

template<typename T>
const std::shared_ptr<Mesh> ObjectManager::createMesh(const std::string& alias, FaceWinding winding) {
    auto meshesIt = this->meshCache.find(alias);
    if (meshesIt == this->meshCache.end()) {
        std::vector<std::shared_ptr<Mesh>> meshes = { nullptr, nullptr };
        meshesIt = this->meshCache.emplace(alias, meshes).first;
    }

    auto& mesh = meshesIt->second.at(winding);
    if (mesh == nullptr) {
        T meshData;
        int vertexElements = sizeof(meshData.vertices) / sizeof(float);
        int vertexIndices = sizeof(meshData.faces) / sizeof(int);

        if (winding == FaceWinding::WINDING_COUNTER_CLOCKWISE) {
            // Flip normals
            for (int i = 0; i < vertexElements; i += 3) {
                meshData.normals[i + 0] = -meshData.normals[i + 0];
                meshData.normals[i + 1] = -meshData.normals[i + 1];
                meshData.normals[i + 2] = -meshData.normals[i + 2];
            }
            // Flip face winding
            for (int i = 0; i < vertexIndices; i += 3) {
                std::swap(meshData.faces[i + 0], meshData.faces[i + 2]);
            }
        }

        mesh = std::make_shared<Mesh>(&meshData, vertexElements / 3, vertexIndices / 3);
    }

    return mesh;
}

}  // namespace Graphene
