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

    struct Mesh
    {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texcoords;
        std::vector<uint32_t> indices;
    };

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

    struct Object
    {
        std::string name;
        glm::mat4 matrix;
        std::vector<size_t> meshIndices;
    };

    struct Animation
    {
        std::string name;
        float duration;
        std::vector<size_t> objectIndices;
    };

    struct Texture
    {
        std::string uri;
    };

    struct Material
    {
        std::string name;
        size_t textureIndex;
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
        std::vector<Object> objects;
        std::vector<Animation> animations;
        std::vector<Texture> textures;
        std::vector<Material> materials;

        bool loadBuffers();
        bool loadBufferViews();
        bool loadAccessors();
        bool loadObjects();
        bool loadAnimations();
        bool loadTextures();  // <-- adicionar
        bool loadMaterials(); // <-- adicionar
        std::vector<float> getAttributeData(size_t accessorIndex);
        bool loadMeshes(std::vector<Mesh> &meshes);
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
            file >> gltf;
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
        if (!loadBuffers() || !loadBufferViews() || !loadAccessors())
        {
            return false;
        }

        if (!loadObjects() || !loadAnimations() || !loadTextures() || !loadMaterials())
        {
            return false;
        }

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

    bool GLTFLoader::loadObjects()
    {
        if (gltf.find("nodes") == gltf.end())
        {
            return false;
        }

        const auto &nodes = gltf["nodes"];
        objects.reserve(nodes.size());

        for (const auto &node : nodes)
        {
            Object object;

            if (node.find("name") != node.end())
            {
                object.name = node["name"].get<std::string>();
            }

            if (node.find("matrix") != node.end())
            {
                std::vector<float> matrixData = node["matrix"].get<std::vector<float>>();
                object.matrix = glm::make_mat4(matrixData.data());
            }
            else
            {
                object.matrix = glm::mat4(1.0f);
            }

            if (node.find("mesh") != node.end())
            {
                object.meshIndices.push_back(node["mesh"].get<size_t>());
            }

            objects.push_back(std::move(object));
        }

        return true;
    }

    bool GLTFLoader::loadAnimations()
    {
        if (gltf.find("animations") == gltf.end())
        {
            return false;
        }

        const auto &animationsJson = gltf["animations"];
        animations.reserve(animationsJson.size());

        for (const auto &animationJson : animationsJson)
        {
            Animation animation;

            if (animationJson.find("name") != animationJson.end())
            {
                animation.name = animationJson["name"].get<std::string>();
            }

            if (animationJson.find("channels") != animationJson.end() && animationJson.find("samplers") != animationJson.end())
            {
                const auto &channels = animationJson["channels"];
                const auto &samplers = animationJson["samplers"];
                float maxTime = 0.0f;

                for (const auto &channel : channels)
                {
                    if (channel.find("target") != channel.end())
                    {
                        const auto &target = channel["target"];

                        if (target.find("node") != target.end())
                        {
                            size_t nodeIndex = target["node"].get<size_t>();
                            animation.objectIndices.push_back(nodeIndex);
                        }
                    }
                }

                for (const auto &sampler : samplers)
                {
                    if (sampler.find("input") != sampler.end())
                    {
                        size_t inputAccessorIndex = sampler["input"].get<size_t>();
                        const Accessor &inputAccessor = accessors[inputAccessorIndex];

                        if (inputAccessor.type == "SCALAR")
                        {
                            std::vector<float> inputTimes = getAttributeData(inputAccessorIndex);
                            if (!inputTimes.empty())
                            {
                                maxTime = std::max(maxTime, inputTimes.back());
                            }
                        }
                    }
                }

                animation.duration = maxTime;
            }

            animations.push_back(std::move(animation));
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
            return false;
        }

        const auto &materialsJson = gltf["materials"];
        materials.reserve(materialsJson.size());

        for (const auto &materialJson : materialsJson)
        {
            Material material;

            if (materialJson.find("name") != materialJson.end())
            {
                material.name = materialJson["name"].get<std::string>();
            }

            if (materialJson.find("pbrMetallicRoughness") != materialJson.end())
            {
                const auto &pbrMetallicRoughnessJson = materialJson["pbrMetallicRoughness"];

                if (pbrMetallicRoughnessJson.find("baseColorTexture") != pbrMetallicRoughnessJson.end())
                {
                    const auto &baseColorTextureJson = pbrMetallicRoughnessJson["baseColorTexture"];

                    if (baseColorTextureJson.find("index") != baseColorTextureJson.end())
                    {
                        material.textureIndex = baseColorTextureJson["index"].get<size_t>();
                    }
                }
            }

            materials.push_back(std::move(material));
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
                if (accessor.componentType == 5126)
                { // FLOAT
                    value = *reinterpret_cast<const float *>(&buffer[srcOffset]);
                }
                else
                {
                    // Handle other component types if needed (e.g., normalize integer types)
                    throw std::runtime_error("Unsupported component type for attribute data.");
                }
                attributeData[i * numComponents + j] = value;
                srcOffset += componentSize;
            }
            srcOffset += byteStride - componentSize * numComponents;
        }

        return attributeData;
    }

    // gltf_loader.cpp

    bool GLTFLoader::loadMeshes(std::vector<Mesh> &meshes)
    {
        if (gltf.find("meshes") == gltf.end())
        {
            std::cerr << "No meshes found in the glTF file." << std::endl;
            return false;
        }

        for (const auto &meshData : gltf["meshes"])
        {
            Mesh mesh;

            for (const auto &primitiveData : meshData["primitives"])
            {
                if (primitiveData.find("attributes") == primitiveData.end())
                {
                    std::cerr << "No attributes found in the mesh primitive." << std::endl;
                    return false;
                }

                const auto &attributes = primitiveData["attributes"];

                if (attributes.find("POSITION") != attributes.end())
                {
                    size_t accessorIndex = attributes["POSITION"].get<size_t>();
                    std::vector<float> positions = getAttributeData(accessorIndex);

                    for (size_t i = 0; i < positions.size(); i += 3)
                    {
                        mesh.positions.emplace_back(positions[i], positions[i + 1], positions[i + 2]);
                    }
                }

                if (attributes.find("NORMAL") != attributes.end())
                {
                    size_t accessorIndex = attributes["NORMAL"].get<size_t>();
                    std::vector<float> normals = getAttributeData(accessorIndex);

                    for (size_t i = 0; i < normals.size(); i += 3)
                    {
                        mesh.normals.emplace_back(normals[i], normals[i + 1], normals[i + 2]);
                    }
                }

                if (attributes.find("TEXCOORD_0") != attributes.end())
                {
                    size_t accessorIndex = attributes["TEXCOORD_0"].get<size_t>();
                    std::vector<float> texcoords = getAttributeData(accessorIndex);

                    for (size_t i = 0; i < texcoords.size(); i += 2)
                    {
                        mesh.texcoords.emplace_back(texcoords[i], texcoords[i + 1]);
                    }
                }

                if (primitiveData.find("indices") != primitiveData.end())
                {
                    size_t accessorIndex = primitiveData["indices"].get<size_t>();
                    std::vector<float> indices = getAttributeData(accessorIndex);

                    for (float index : indices)
                    {
                        mesh.indices.push_back(static_cast<uint32_t>(index));
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