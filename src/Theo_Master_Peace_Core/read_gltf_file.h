#pragma once
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "base64.h"

namespace gltf_loader
{

    struct BufferView
    {
        size_t buffer;
        size_t byteOffset;
        size_t byteLength;
        size_t byteStride;
    };

    struct Accessor
    {
        size_t bufferView;
        size_t byteOffset;
        size_t componentType;
        size_t count;
        std::string type;
    };

    struct Mesh
    {
        std::string name;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoords;
        std::vector<unsigned int> indices;
    };

    struct Node
    {
        std::string name;
        glm::mat4 matrix;
        std::vector<size_t> meshIndices, childrenIndices;
        nlohmann::json extensions, extras;
    };

    struct AnimationChannel
    {
        size_t samplerIndex;
        size_t targetNodeIndex;
        std::string targetPath;
    };

    struct AnimationSampler
    {
        size_t inputAccessorIndex;
        size_t outputAccessorIndex;
        std::string interpolation;
    };

    struct Animation
    {
        std::string name;
        float duration;
        std::vector<AnimationChannel> channels;
        std::vector<AnimationSampler> samplers;
    };

    struct Texture
    {
        std::string uri;
    };

    struct Material
    {
        std::string name;
        size_t textureIndex = 0;
        vec4 baseColorFactor = vec4(0, 0, 0, 0);
        float metallicFactor = 0;
        float roughnessFactor = 0;
        size_t metallicRoughnessTextureIndex = 0;
        size_t normalTextureIndex = 0;
        size_t occlusionTexture = 0;
        size_t emissiveTexture = 0;
        vec3 emissiveFactor = vec3(0, 0, 0);
        string alphaMode;
        float alphaCutoff = 0;
        bool doubleSided = 0;
        nlohmann::json extensions, extras;
    };

    class GLTFLoader
    {
    public:
        GLTFLoader(const std::string &filename);
        bool load();

        nlohmann::json gltf;
        std::string baseDir;
        std::vector<std::vector<uint8_t>> buffersData;
        std::vector<BufferView> bufferViews;
        std::vector<Accessor> accessors;
        std::vector<Mesh> meshes;
        std::vector<Node> nodes;
        std::vector<Animation> animations;
        std::vector<Texture> textures;
        std::vector<Material> materials;

        bool loadBuffers();
        bool loadBufferViews();
        bool loadAccessors();
        bool loadNodes();
        bool loadAnimations();
        bool loadTextures();  // <-- adicionar
        bool loadMaterials(); // <-- adicionar
        std::vector<float> getAttributeData(size_t accessorIndex);
        bool loadMeshes();
        std::vector<uint8_t> getBufferData(size_t accessorIndex);
    };

    GLTFLoader::GLTFLoader(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "Failed to open glTF file: " << filename << std::endl;
            return;
        }

        try
        {
            // file >> gltf;
            gltf = json::parse(file);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Failed to parse glTF file: " << e.what() << std::endl;
            return;
        }

        // Get base directory from the file path
        baseDir = std::filesystem::path(filename).parent_path().string();
        if (!baseDir.empty())
        {
            baseDir += "/";
        }
    }

    bool GLTFLoader::load()
    {
        loadBuffers();
        loadBufferViews();
        loadAccessors();
        loadMeshes();
        loadNodes();
        loadAnimations();
        loadTextures();
        loadMaterials();

        return true;
    }

    bool GLTFLoader::loadBuffers()
    {
        if (gltf.find("buffers") == gltf.end())
        {
            std::cerr << "No buffers found in the glTF file." << std::endl;
            return false;
        }

        buffersData.resize(gltf["buffers"].size());

        for (size_t i = 0; i < gltf["buffers"].size(); ++i)
        {
            const auto &buffer = gltf["buffers"][i];
            if (buffer.find("uri") != buffer.end())
            {
                const std::string &uri = buffer["uri"].get<std::string>();
                if (uri.find("data:application/octet-stream;base64,") == 0)
                {
                    std::string base64Data = uri.substr(strlen("data:application/octet-stream;base64,"));
                    std::string decodedData = base64_decode(base64Data);
                    buffersData[i] = std::vector<uint8_t>(decodedData.begin(), decodedData.end());
                }
                else
                {
                    std::string bufferPath = baseDir + "/" + uri;
                    std::ifstream bufferFile(bufferPath, std::ios::binary);
                    if (!bufferFile.is_open())
                    {
                        std::cerr << "Failed to open the buffer file: " << bufferPath << std::endl;
                        return false;
                    }
                    bufferFile.unsetf(std::ios::skipws);
                    buffersData[i] = std::vector<uint8_t>(std::istream_iterator<uint8_t>(bufferFile), std::istream_iterator<uint8_t>());
                    bufferFile.close();
                }
            }
            else
            {
                std::cerr << "Buffer URI not found." << std::endl;
                return false;
            }

            if (buffersData[i].size() != buffer["byteLength"].get<size_t>())
            {
                std::cerr << "Buffer data size mismatch." << std::endl;
                return false;
            }
        }

        return true;
    }

    bool GLTFLoader::loadBufferViews()
    {
        if (gltf.find("bufferViews") == gltf.end())
        {
            std::cerr << "No buffer views found in the glTF file." << std::endl;
            return false;
        }

        bufferViews.resize(gltf["bufferViews"].size());

        for (size_t i = 0; i < gltf["bufferViews"].size(); ++i)
        {
            const auto &bufferViewData = gltf["bufferViews"][i];
            BufferView bufferView;
            bufferView.buffer = bufferViewData["buffer"].get<size_t>();
            bufferView.byteOffset = bufferViewData.value("byteOffset", 0);
            bufferView.byteLength = bufferViewData["byteLength"].get<size_t>();
            bufferView.byteStride = bufferViewData.value("byteStride", 0);

            if (bufferView.buffer >= buffersData.size())
            {
                std::cerr << "Invalid buffer index in buffer view." << std::endl;
                return false;
            }

            bufferViews[i] = bufferView;
        }

        return true;
    }

glm::mat4 jsonToMat4(const nlohmann::json &jsonArray)
{
    glm::mat4 matrix(1.0f);
    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            matrix[i][j] = jsonArray[i * 4 + j].get<float>();
        }
    }
    return matrix;
}

    bool GLTFLoader::loadNodes()
    {
        if (gltf.find("nodes") == gltf.end())
        {
            return false;
        }

        const auto &gltfNodes = gltf["nodes"];
        nodes.resize(gltfNodes.size());

        for (size_t i = 0; i < gltfNodes.size(); ++i)
        {
            const auto &gltfNode = gltfNodes[i];
            Node &node = nodes[i];

            if (gltfNode.find("name") != gltfNode.end())
            {
                node.name = gltfNode["name"].get<std::string>();
            }

            if (gltfNode.find("matrix") != gltfNode.end())
            {
                const auto &matrixData = gltfNode["matrix"];
                glm::mat4 matrix = jsonToMat4(matrixData);
                node.matrix = matrix;
            }
            else
            {
                node.matrix = glm::mat4(1.0f);
            }

            if (gltfNode.find("mesh") != gltfNode.end())
            {
                node.meshIndices.push_back(gltfNode["mesh"].get<size_t>());
            }

            if (gltfNode.find("children") != gltfNode.end())
            {
                const auto &children = gltfNode["children"];
                for (const auto &childIndex : children)
                {
                    node.childrenIndices.push_back(childIndex.get<size_t>());
                }
            }

            if (gltfNode.find("extensions") != gltfNode.end())
            {
                node.extensions = gltfNode["extensions"];
            }

            if (gltfNode.find("extras") != gltfNode.end())
            {
                node.extras = gltfNode["extras"];
            }
        }

        return true;
    }

    bool GLTFLoader::loadAccessors()
    {
        if (gltf.find("accessors") == gltf.end())
        {
            std::cerr << "No accessors found in the glTF file." << std::endl;
            return false;
        }

        accessors.resize(gltf["accessors"].size());

        for (size_t i = 0; i < gltf["accessors"].size(); ++i)
        {
            const auto &accessorData = gltf["accessors"][i];
            Accessor accessor;
            accessor.bufferView = accessorData["bufferView"].get<size_t>();
            accessor.byteOffset = accessorData.value("byteOffset", 0);
            accessor.componentType = accessorData["componentType"].get<size_t>();
            accessor.count = accessorData["count"].get<size_t>();
            accessor.type = accessorData["type"].get<std::string>();

            if (accessor.bufferView >= bufferViews.size())
            {
                std::cerr << "Invalid buffer view index in accessor." << std::endl;
                return false;
            }

            accessors[i] = accessor;
        }

        return true;
    }

    bool GLTFLoader::loadAnimations()
    {
        if (gltf.find("animations") == gltf.end())
        {
            return true;
        }

        const auto &animationsJson = gltf["animations"];
        animations.reserve(animationsJson.size());

        for (const auto &animationJson : animationsJson)
        {
            Animation animation;

            if (animationJson.contains("name"))
            {
                animation.name = animationJson["name"];
            }

            const auto &channelsJson = animationJson["channels"];
            const auto &samplersJson = animationJson["samplers"];

            animation.channels.reserve(channelsJson.size());
            animation.samplers.reserve(samplersJson.size());

            for (const auto &channelJson : channelsJson)
            {
                AnimationChannel channel;
                channel.samplerIndex = channelJson["sampler"];
                channel.targetNodeIndex = channelJson["target"]["node"];
                channel.targetPath = channelJson["target"]["path"];
                animation.channels.push_back(channel);
            }

            for (const auto &samplerJson : samplersJson)
            {
                AnimationSampler sampler;
                sampler.inputAccessorIndex = samplerJson["input"];
                sampler.outputAccessorIndex = samplerJson["output"];
                sampler.interpolation = samplerJson["interpolation"];
                animation.samplers.push_back(sampler);
            }

            animations.push_back(animation);
        }

        return true;
    }

    bool GLTFLoader::loadTextures()
    {
        if (gltf.find("textures") == gltf.end())
        {
            return false;
        }

        const auto &texturesJson = gltf["textures"];
        textures.reserve(texturesJson.size());

        for (const auto &textureJson : texturesJson)
        {
            Texture texture;

            if (textureJson.find("source") != textureJson.end())
            {
                size_t imageIndex = textureJson["source"].get<size_t>();

                if (gltf.find("images") != gltf.end())
                {
                    const auto &imagesJson = gltf["images"];

                    if (imageIndex < imagesJson.size())
                    {
                        const auto &imageJson = imagesJson[imageIndex];

                        if (imageJson.find("uri") != imageJson.end())
                        {
                            texture.uri = baseDir + "/" + imageJson["uri"].get<std::string>();
                        }
                    }
                }
            }

            textures.push_back(std::move(texture));
        }

        return true;
    }

    bool GLTFLoader::loadMaterials()
    {
        if (gltf.find("materials") == gltf.end())
        {
            return true; // No materials to load
        }

        const auto &materialsJson = gltf["materials"];
        materials.reserve(materialsJson.size());

        for (const auto &materialJson : materialsJson)
        {
            Material material;

            if (materialJson.contains("name"))
            {
                material.name = materialJson["name"].get<std::string>();
            }

            if (materialJson.contains("pbrMetallicRoughness"))
            {
                const auto &pbr = materialJson["pbrMetallicRoughness"];

                if (pbr.contains("baseColorFactor"))
                {
                    material.baseColorFactor = glm::vec4(pbr["baseColorFactor"][0].get<float>(), pbr["baseColorFactor"][1].get<float>(), pbr["baseColorFactor"][2].get<float>(), pbr["baseColorFactor"][3].get<float>());
                }

                if (pbr.contains("metallicFactor"))
                {
                    material.metallicFactor = pbr["metallicFactor"].get<float>();
                }

                if (pbr.contains("roughnessFactor"))
                {
                    material.roughnessFactor = pbr["roughnessFactor"].get<float>();
                }

                if (pbr.contains("metallicRoughnessTexture"))
                {
                    material.metallicRoughnessTextureIndex = pbr["metallicRoughnessTexture"]["index"].get<size_t>();
                }
            }

            if (materialJson.contains("normalTexture"))
            {
                material.normalTextureIndex = materialJson["normalTexture"]["index"].get<size_t>();
            }

            if (materialJson.contains("occlusionTexture"))
            {
                material.occlusionTexture = materialJson["occlusionTexture"]["index"].get<size_t>();
            }

            if (materialJson.contains("emissiveTexture"))
            {
                material.emissiveTexture = materialJson["emissiveTexture"]["index"].get<size_t>();
            }

            if (materialJson.contains("emissiveFactor"))
            {
                material.emissiveFactor = glm::vec3(materialJson["emissiveFactor"][0].get<float>(), materialJson["emissiveFactor"][1].get<float>(), materialJson["emissiveFactor"][2].get<float>());
            }

            if (materialJson.contains("alphaMode"))
            {
                material.alphaMode = materialJson["alphaMode"].get<std::string>();
            }

            if (materialJson.contains("alphaCutoff"))
            {
                material.alphaCutoff = materialJson["alphaCutoff"].get<float>();
            }

            if (materialJson.contains("doubleSided"))
            {
                material.doubleSided = materialJson["doubleSided"].get<bool>();
            }

            if (materialJson.contains("extensions"))
            {
                material.extensions = materialJson["extensions"];
            }

            if (materialJson.contains("extras"))
            {
                material.extras = materialJson["extras"];
            }

            materials.push_back(material);
        }

        return true;
    }

    std::vector<float> GLTFLoader::getAttributeData(size_t accessorIndex)
    {
        const Accessor &accessor = accessors[accessorIndex];
        const BufferView &bufferView = bufferViews[accessor.bufferView];
        const std::vector<uint8_t> &buffer = buffersData[bufferView.buffer];

        size_t componentSize;
        switch (accessor.componentType)
        {
        case 5120: // BYTE
            componentSize = 1;
            break;
        case 5121: // UNSIGNED_BYTE
            componentSize = 1;
            break;
        case 5122: // SHORT
            componentSize = 2;
            break;
        case 5123: // UNSIGNED_SHORT
            componentSize = 2;
            break;
        case 5125: // UNSIGNED_INT
            componentSize = 4;
            break;
        case 5126: // FLOAT
            componentSize = 4;
            break;
        default:
            throw std::runtime_error("Unsupported component type.");
        }

        size_t numComponents;
        if (accessor.type == "SCALAR")
        {
            numComponents = 1;
        }
        else if (accessor.type == "VEC2")
        {
            numComponents = 2;
        }
        else if (accessor.type == "VEC3")
        {
            numComponents = 3;
        }
        else if (accessor.type == "VEC4")
        {
            numComponents = 4;
        }
        else
        {
            throw std::runtime_error("Unsupported accessor type.");
        }

        size_t byteStride = bufferView.byteStride != 0 ? bufferView.byteStride : componentSize * numComponents;
        size_t srcOffset = bufferView.byteOffset + accessor.byteOffset;
        std::vector<float> attributeData(accessor.count * numComponents);

        for (size_t i = 0; i < accessor.count; ++i)
        {
            for (size_t j = 0; j < numComponents; ++j)
            {
                float value;
                switch (accessor.componentType)
                {
                case 5126: // FLOAT
                    value = *reinterpret_cast<const float *>(&buffer[srcOffset]);
                    break;
                case 5123: // UNSIGNED_SHORT
                    value = static_cast<float>(*reinterpret_cast<const uint16_t *>(&buffer[srcOffset])) / 65535.0f;
                    break;
                default:
                    throw std::runtime_error("Unsupported component type for attribute data.");
                }
                attributeData[i * numComponents + j] = value;
                srcOffset += componentSize;
            }
            srcOffset += byteStride - componentSize * numComponents;
        }

        return attributeData;
    }

    bool GLTFLoader::loadMeshes()
    {
        if (!gltf.contains("meshes"))
        {
            return false;
        }

        for (const auto &meshJson : gltf["meshes"])
        {
            Mesh mesh;

            if (meshJson.contains("name"))
            {
                mesh.name = meshJson["name"].get<std::string>();
            }

            for (const auto &primitive : meshJson["primitives"])
            {
                if (!primitive.contains("attributes"))
                {
                    continue;
                }

                const auto &attributes = primitive["attributes"];

                if (attributes.contains("POSITION"))
                {
                    size_t positionAccessorIndex = attributes["POSITION"].get<size_t>();
                    std::vector<float> positionData = getAttributeData(positionAccessorIndex);

                    for (size_t i = 0; i < positionData.size(); i += 3)
                    {
                        mesh.positions.emplace_back(positionData[i], positionData[i + 1], positionData[i + 2]);
                    }
                }

                if (attributes.contains("NORMAL"))
                {
                    size_t normalAccessorIndex = attributes["NORMAL"].get<size_t>();
                    std::vector<float> normalData = getAttributeData(normalAccessorIndex);

                    for (size_t i = 0; i < normalData.size(); i += 3)
                    {
                        mesh.normals.emplace_back(normalData[i], normalData[i + 1], normalData[i + 2]);
                    }
                }

                if (attributes.contains("TEXCOORD_0"))
                {
                    size_t texcoordAccessorIndex = attributes["TEXCOORD_0"].get<size_t>();
                    std::vector<float> texcoordData = getAttributeData(texcoordAccessorIndex);

                    for (size_t i = 0; i < texcoordData.size(); i += 2)
                    {
                        mesh.texcoords.emplace_back(texcoordData[i], texcoordData[i + 1]);
                    }
                }

                if (primitive.contains("indices"))
                {
                    size_t indexAccessorIndex = primitive["indices"].get<size_t>();
                    const Accessor &indexAccessor = accessors[indexAccessorIndex];
                    std::vector<uint8_t> indexBufferData = getBufferData(indexAccessorIndex);

                    switch (indexAccessor.componentType)
                    {
                    case 5121:
                    { // UNSIGNED_BYTE
                        std::vector<uint8_t> indices(indexBufferData.begin(), indexBufferData.end());
                        mesh.indices.insert(mesh.indices.end(), indices.begin(), indices.end());
                        break;
                    }
                    case 5123:
                    { // UNSIGNED_SHORT
                        std::vector<uint16_t> indices(reinterpret_cast<const uint16_t *>(indexBufferData.data()), reinterpret_cast<const uint16_t *>(indexBufferData.data() + indexBufferData.size()));
                        mesh.indices.insert(mesh.indices.end(), indices.begin(), indices.end());
                        break;
                    }
                    case 5125:
                    { // UNSIGNED_INT
                        std::vector<uint32_t> indices(reinterpret_cast<const uint32_t *>(indexBufferData.data()), reinterpret_cast<const uint32_t *>(indexBufferData.data() + indexBufferData.size()));
                        mesh.indices.insert(mesh.indices.end(), indices.begin(), indices.end());
                        break;
                    }
                    default:
                        throw std::runtime_error("Unsupported index component type.");
                    }
                }
            }

            meshes.push_back(mesh);
        }

        return true;
    }

    std::vector<uint8_t> GLTFLoader::getBufferData(size_t accessorIndex)
    {
        const Accessor &accessor = accessors[accessorIndex];
        const BufferView &bufferView = bufferViews[accessor.bufferView];
        const std::vector<uint8_t> &buffer = buffersData[bufferView.buffer];

        size_t dataOffset = bufferView.byteOffset + accessor.byteOffset;
        size_t dataSize = accessor.count * (bufferView.byteStride != 0 ? bufferView.byteStride : accessor.type.size() * sizeof(float));

        std::vector<uint8_t> data(buffer.begin() + dataOffset, buffer.begin() + dataOffset + dataSize);
        return data;
    }
};