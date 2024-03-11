#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "base64.h"
#include "benchmark.h"

bool file_exist(string nome)
{
    ifstream file(nome.c_str());
    if (!file)
    {
        return false;
    }
    return true;
}

#ifndef ANIMATION_FPS_COUNT
#define ANIMATION_FPS_COUNT 20
#endif

#ifndef MAX_BONE_INFLUENCE
#define MAX_BONE_INFLUENCE 4
#endif

namespace gltf_loader
{
    struct scene
    {
        string name;
        std::vector<size_t> nodesIndices;
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
        size_t bufferView = 0;
        size_t byteOffset = 0;
        size_t componentType = 0;
        size_t count;
        std::vector<float> min = {}, max = {};
        std::string type;
        nlohmann::json extensions, extras;
    };

    struct SubMesh
    {
        bool skin = false;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec4> colors;
        std::vector<glm::vec2> texcoords;
        std::vector<unsigned int> indices;
        std::vector<std::vector<size_t>> BoneIDs;
        std::vector<std::vector<float>> Weights;
        size_t material = 0;
    };

    struct Mesh
    {
        std::string name;
        std::vector<SubMesh> sub_meshes;
        bool have_skin = false;
        size_t skin;
        nlohmann::json extensions, extras;
    };

    struct Skin
    {
        std::string name;
        std::vector<size_t> jointIndices;
        std::vector<glm::mat4> inverseBindMatrices;
    };

    // Enumeração para os tipos de luz
    enum class LightType
    {
        Directional,
        Point,
        Spot,
        // Adicione outros tipos de luz, se necessário
    };

    // Struct para representar uma luz em um arquivo glTF
    struct Light
    {
        std::string name;    // Nome da luz
        LightType type;      // Tipo da luz (direcional, ponto, spot, etc.)
        float intensity;     // Intensidade da luz
        glm::vec3 color;     // Cor da luz
        glm::vec3 position;  // Posição da luz (apenas para luzes do tipo ponto e spot)
        glm::vec3 direction; // Direção da luz (apenas para luzes do tipo direcional e spot)
        float range;         // Alcance da luz (apenas para luzes do tipo ponto e spot)
        float spotAngle;     // Ângulo do spot (apenas para luzes do tipo spot)
    };

    struct Node
    {
        std::string name = "";
        size_t id = -1;
        glm::mat4 matrix = glm::mat4(1);
        glm::vec3 translation = glm::vec3(0, 0, 0);
        glm::quat rotation = glm::quat(1, 0, 0, 0);
        glm::vec3 scale = glm::vec3(1, 1, 1);
        std::vector<size_t> meshIndices, childrenIndices;
        bool have_skin = false;
        size_t skin;
        nlohmann::json extensions, extras;
    };

    struct AnimationKeyFrame
    {
        size_t targetNodeIndex = 0;
        bool has_position = false, has_scale = false, has_rotation = false;
        vec3 position = glm::vec3(0, 0, 0);
        vec3 scale = glm::vec3(1, 1, 1);
        quat rotation = glm::quat(1, 0, 0, 0);
    };

    struct AnimationChannel
    {
        size_t samplerIndex = 0;
        size_t targetNodeIndex = 0;
        std::string targetPath = "";
    };

    struct AnimationSampler
    {
        float start_time = 0, duration = 0;
        size_t inputAccessorIndex = 0;
        size_t outputAccessorIndex = 0;
        std::string interpolation = "";
    };

    struct Animation
    {
        std::string name = "";
        float start_time = 0, duration = 0;
        std::vector<AnimationChannel> channels = {};
        std::vector<AnimationSampler> samplers = {};
        std::vector<std::vector<AnimationKeyFrame>> keyFrames = {};
        nlohmann::json extensions, extras;
    };

    struct Texture
    {
        std::string uri;
    };

    struct Material
    {
        std::string name;
        int textureIndex = -1;
        glm::vec4 baseColorFactor = glm::vec4(0, 0, 0, 0);
        float metallicFactor = 0;
        float roughnessFactor = 0;
        int metallicRoughnessTextureIndex = -1;
        int normalTextureIndex = -1;
        int occlusionTexture = -1;
        int emissiveTexture = -1;
        glm::vec3 emissiveFactor = glm::vec3(0, 0, 0);
        std::string alphaMode;
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
        std::vector<scene> scenes;
        std::vector<Node> nodes;
        std::vector<Animation> animations;
        std::vector<Texture> textures;
        std::vector<Skin> skins;
        std::vector<Material> materials;
        std::vector<Light> Lights;

        bool loadBuffers();
        bool loadBufferViews();
        bool loadAccessors();
        bool loadScenes();
        bool loadNodes();
        bool loadLights();
        glm::vec2 getAnimationTimeDuration(Animation &channel);
        void getAnimationKeyFrame(Animation animation, const AnimationChannel &channel, float time, AnimationKeyFrame &keyFrame);
        bool loadAnimations();
        bool loadTextures();
        bool loadMaterials();
        std::vector<std::vector<float>> getWeightsData(size_t accessorIndex);
        std::vector<std::vector<size_t>> getBoneIDsData(size_t accessorIndex);
        std::vector<glm::mat4> getInverseBindMatrices(size_t accessorIndex);
        bool loadSkins();
        bool assigneSkinsToMeshes();
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
                    std::string bufferPath = baseDir + uri;
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

    bool GLTFLoader::loadScenes()
    {
        if (!gltf.contains("scenes"))
        {
            std::cerr << "GLTF file does not contain scenes." << std::endl;
            return false;
        }

        const auto &scenesArray = gltf["scenes"];

        for (const auto &sceneJson : scenesArray)
        {
            scene sceneData;

            if (sceneJson.contains("name"))
            {
                sceneData.name = sceneJson["name"].get<std::string>();
            }

            if (sceneJson.contains("nodes"))
            {
                const auto &nodesArray = sceneJson["nodes"];

                for (const auto &nodeIndex : nodesArray)
                {
                    sceneData.nodesIndices.push_back(nodeIndex.get<size_t>());
                }
            }

            scenes.push_back(sceneData);
        }

        return true;
    }

    glm::vec3 jsonToVec3(const nlohmann::json &json)
    {
        if (json.is_array() && json.size() >= 3)
        {
            return glm::vec3(json[0].get<float>(), json[1].get<float>(), json[2].get<float>());
        }
        else
        {
            return glm::vec3(0.0f);
        }
    }

    bool GLTFLoader::loadLights()
    {
        if (!gltf.contains("extensions"))
        {
            if (!gltf["extensions"].contains("KHR_lights_punctual"))
            {
                return true;
            }
        }

        const nlohmann::json &lightsArray = gltf["extensions"]["KHR_lights_punctual"];

        return true; // Indica que a carga das luzes foi bem-sucedida
    }

    bool GLTFLoader::loadNodes()
    {
        if (!gltf.contains("nodes"))
        {
            std::cerr << "GLTF file does not contain nodes." << std::endl;
            return false;
        }

        const auto &nodesArray = gltf["nodes"];

        int i = 0;
        for (const auto &nodeJson : nodesArray)
        {

            Node nodeData;

            nodeData.id = i;

            if (nodeJson.contains("name"))
            {
                nodeData.name = nodeJson["name"].get<std::string>();
            }

            if (nodeJson.contains("matrix"))
            {
                const auto &matrixData = nodeJson["matrix"];
                glm::mat4 matrix = jsonToMat4(matrixData);
                nodeData.matrix = matrix;
            }

            if (nodeJson.contains("translation"))
            {
                const auto &translation = nodeJson["translation"];
                glm::vec3 pos = vec3(translation[0], translation[1], translation[2]);
                nodeData.translation = pos;
            }

            if (nodeJson.contains("rotation"))
            {
                const auto &rotation = nodeJson["rotation"];
                glm::quat q = glm::quat(rotation[3], rotation[0], rotation[1], rotation[2]);
                nodeData.rotation = q;
            }

            if (nodeJson.contains("scale"))
            {
                const auto &scale = nodeJson["scale"];
                glm::vec3 sca = glm::vec3(scale[0], scale[1], scale[2]);
                nodeData.scale = sca;
            }

            if (nodeJson.contains("mesh"))
            {
                nodeData.meshIndices.push_back(nodeJson["mesh"].get<size_t>());
            }

            if (nodeJson.contains("skin"))
            {
                nodeData.have_skin = true;
                nodeData.skin = nodeJson["skin"].get<size_t>();
            }

            if (nodeJson.contains("children"))
            {
                vector<int> childrenArray = nodeJson["children"].get<vector<int>>();
                for (size_t childIndex : childrenArray)
                {
                    nodeData.childrenIndices.push_back((size_t)childIndex);
                }
            }

            if (nodeJson.contains("extensions"))
            {
                nodeData.extensions = nodeJson["extensions"];
            }

            if (nodeJson.contains("extras"))
            {
                nodeData.extras = nodeJson["extras"];
            }

            nodes.push_back(nodeData);

            i++;
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
            /**/
            if (accessorData.contains("min"))
            {
                accessor.min = accessorData["min"].get<vector<float>>();
            }
            if (accessorData.contains("max"))
            {
                accessor.max = accessorData["max"].get<vector<float>>();
            }
            accessor.type = accessorData["type"].get<std::string>();

            if (accessor.bufferView >= bufferViews.size())
            {
                std::cerr << "Invalid buffer view index in accessor." << std::endl;
                return false;
            }

            if (accessorData.contains("extensions"))
            {
                accessor.extensions = accessorData["extensions"];
            }

            if (accessorData.contains("extras"))
            {
                accessor.extras = accessorData["extras"];
            }

            accessors[i] = accessor;
        }

        return true;
    }

    glm::vec2 GLTFLoader::getAnimationTimeDuration(Animation &animation)
    {
        float startTime = std::numeric_limits<float>::max();
        float endTime = std::numeric_limits<float>::min();

        for (const AnimationChannel &channel : animation.channels)
        {
            const AnimationSampler &sampler = animation.samplers[channel.samplerIndex];
            Accessor &inputAccessor = accessors[sampler.inputAccessorIndex];
            std::vector<float> input = getAttributeData(inputAccessor.bufferView);

            if (!input.empty())
            {

                // Find the actual start and end times in this channel
                float channelStartTime = input[0];
                float channelEndTime = input[input.size() - 1];

                // Update global start and end times
                startTime = std::min(startTime, channelStartTime);
                endTime = std::max(endTime, channelEndTime);
            }
        }

        // Calculate duration based on the global start and end times
        float duration = endTime - startTime;

        if (startTime > duration)
        {
            animation.start_time = duration;
            animation.duration = startTime;
            return glm::vec2(duration, startTime);
        }

        animation.start_time = startTime;
        animation.duration = duration;
        return glm::vec2(startTime, duration);
    }

    void GLTFLoader::getAnimationKeyFrame(Animation animation, const AnimationChannel &channel, float time, AnimationKeyFrame &keyFrame)
    {
        keyFrame.targetNodeIndex = channel.targetNodeIndex;

        // Find the sampler associated with this channel
        if (channel.samplerIndex >= animation.samplers.size())
        {
            return;
        }

        const AnimationSampler &sampler = animation.samplers[channel.samplerIndex];

        // Retrieve the input and output accessors
        if (sampler.inputAccessorIndex >= accessors.size() || sampler.outputAccessorIndex >= accessors.size())
        {
            return;
        }

        Accessor &inputAccessor = accessors[sampler.inputAccessorIndex];
        Accessor &outputAccessor = accessors[sampler.outputAccessorIndex];

        const std::vector<float> input = getAttributeData(inputAccessor.bufferView);
        const std::vector<float> output = getAttributeData(outputAccessor.bufferView);

        // Find the keyframes surrounding the specified time
        size_t index1 = 0, index2 = 0;
        for (size_t i = 0; i < input.size() - 1; ++i)
        {
            if (input[i] <= time && time <= input[i + 1])
            {
                index1 = i;
                index2 = i + 1;
                break;
            }
        }

        // Calculate the interpolation factor
        float t = (time - input[index1]) / (input[index2] - input[index1]);

        // Perform interpolation based on the target path
        if (channel.targetPath == "translation")
        {
            // Interpolate translation
            if (index1 * 3 + 2 < output.size() && index2 * 3 + 2 < output.size())
            {
                glm::vec3 pos1 = glm::make_vec3(&output[index1 * 3]);
                glm::vec3 pos2 = glm::make_vec3(&output[index2 * 3]);
                keyFrame.position = glm::mix(pos1, pos2, t);

                keyFrame.has_position = true;
            }
        }
        else if (channel.targetPath == "rotation")
        {
            // Interpolate rotation as quaternions
            if (index1 * 4 + 3 < output.size() && index2 * 4 + 3 < output.size())
            {
                glm::quat rot1 = glm::make_quat(&output[index1 * 4]);
                glm::quat rot2 = glm::make_quat(&output[index2 * 4]);
                keyFrame.rotation = glm::normalize(glm::slerp(rot1, rot2, t));
                keyFrame.has_rotation = true;
            }
        }
        else if (channel.targetPath == "scale")
        {
            // Interpolate scale
            if (index1 * 3 + 2 < output.size() && index2 * 3 + 2 < output.size())
            {
                glm::vec3 scale1 = glm::make_vec3(&output[index1 * 3]);
                glm::vec3 scale2 = glm::make_vec3(&output[index2 * 3]);
                keyFrame.scale = glm::mix(scale1, scale2, t);
                keyFrame.has_scale = true;
            }
        }

        // Add handling for other target paths as needed
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

            if (animationJson.contains("extensions"))
            {
                animation.extensions = animationJson["extensions"];
            }

            if (animationJson.contains("extras"))
            {
                animation.extras = animationJson["extras"];
            }

            animations.push_back(animation);
        }

        for (int a = 0; a < animations.size(); a++)
        {

            vec2 start_duration = getAnimationTimeDuration(animations[a]);
            float time = 0;

            for (float t = start_duration.x; t < start_duration.x + start_duration.y; t += 1.0 / ANIMATION_FPS_COUNT)
            {
                t = std::min(t, start_duration.x + start_duration.y);

                vector<AnimationKeyFrame> key_frames;
                for (AnimationChannel ac : animations[a].channels)
                {
                    AnimationKeyFrame akf;

                    getAnimationKeyFrame(animations[a], ac, t, akf);

                    if (akf.targetNodeIndex != -1 && (akf.has_position || akf.has_rotation || akf.has_scale))
                    {
                        key_frames.push_back(akf);
                    }
                }
                animations[a].keyFrames.push_back(key_frames);
            }
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
                            texture.uri = baseDir + imageJson["uri"].get<std::string>();
                        }
                        else if (imageJson.find("name") != imageJson.end())
                        {

                            if (imageJson["mimeType"] == "image/png")
                            {
                                if (file_exist(baseDir + imageJson["name"].get<std::string>() + ".png"))
                                {
                                    texture.uri = baseDir + imageJson["name"].get<std::string>() + ".png";
                                }
                                else if (file_exist("resources/Textures/" + imageJson["name"].get<std::string>() + ".png"))
                                {
                                    texture.uri = "resources/Textures/" + imageJson["name"].get<std::string>() + ".png";
                                }
                            }
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

                if (pbr.contains("baseColorTexture"))
                {
                    material.textureIndex = pbr["baseColorTexture"]["index"].get<size_t>();
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

    std::vector<glm::mat4> GLTFLoader::getInverseBindMatrices(size_t accessorIndex)
    {
        const std::vector<float> attributeData = getAttributeData(accessorIndex);
        const Accessor &accessor = accessors[accessorIndex];
        const size_t numComponents = 16; // 4x4 matrix
        const size_t numMatrices = accessor.count;
        std::vector<glm::mat4> inverseBindMatrices(numMatrices);

        if (attributeData.size() != numMatrices * numComponents)
        {
            throw std::runtime_error("Invalid inverse bind matrices data.");
        }

        for (size_t i = 0; i < numMatrices; ++i)
        {
            glm::mat4 inverseBindMatrix(1.0f); // Inicialize com a matriz de identidade

            for (size_t row = 0; row < 4; ++row)
            {
                for (size_t col = 0; col < 4; ++col)
                {
                    size_t dataIdx = i * numComponents + row * 4 + col;
                    inverseBindMatrix[row][col] = attributeData[dataIdx];
                }
            }

            inverseBindMatrices[i] = inverseBindMatrix;
        }

        return inverseBindMatrices;
    }

    bool GLTFLoader::loadSkins()
    {

        if (!gltf.contains("skins"))
        {
            return false; // Não há dados de skins no glTF
        }

        const nlohmann::json &skinsArray = gltf["skins"];
        skins.reserve(skinsArray.size());

        for (const auto &skinData : skinsArray)
        {
            Skin skin;

            if (skinData.contains("name"))
            {
                skin.name = skinData["name"];
            }

            if (skinData.contains("joints"))
            {
                skin.jointIndices = skinData["joints"].get<std::vector<size_t>>();
            }

            /**/
            if (skinData.contains("inverseBindMatrices"))
            {
                size_t accessorIndex = skinData["inverseBindMatrices"];
                skin.inverseBindMatrices = getInverseBindMatrices(accessorIndex);
            }

            skins.push_back(skin);
        }

        return true;
    }

    std::vector<float> GLTFLoader::getAttributeData(size_t accessorIndex)
    {
        const Accessor &accessor = accessors[accessorIndex];
        const BufferView &bufferView = bufferViews[accessor.bufferView];
        const std::vector<uint8_t> &buffer = buffersData[bufferView.buffer];

        size_t componentSize = 0;
        switch (accessor.componentType)
        {
        case 5120: // BYTE
        case 5121: // UNSIGNED_BYTE
            componentSize = 1;
            break;
        case 5122: // SHORT
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

        size_t numComponents = 0;
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
        else if (accessor.type == "MAT4")
        {
            numComponents = 16;
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
                float value = 0.0f;

                if (accessor.componentType == 5126) // FLOAT
                {
                    if (srcOffset + sizeof(float) <= buffer.size())
                    {
                        value = *reinterpret_cast<const float *>(&buffer[srcOffset]);
                    }
                    else
                    {
                        throw std::runtime_error("Buffer overflow while reading FLOAT data.");
                    }
                }
                else if (accessor.componentType == 5123) // UNSIGNED_SHORT
                {
                    if (srcOffset + sizeof(uint16_t) <= buffer.size())
                    {
                        uint16_t uint16Value = *reinterpret_cast<const uint16_t *>(&buffer[srcOffset]);
                        value = static_cast<float>(uint16Value) / 65535.0f;
                        // value = static_cast<float>(uint16Value);
                    }
                    else
                    {
                        throw std::runtime_error("Buffer overflow while reading UNSIGNED_SHORT data.");
                    }
                }
                else if (accessor.componentType == 5125) // UNSIGNED_INT
                {
                    if (srcOffset + sizeof(uint32_t) <= buffer.size())
                    {
                        uint32_t uint32Value = *reinterpret_cast<const uint32_t *>(&buffer[srcOffset]);
                        value = static_cast<int>(uint32Value);
                    }
                    else
                    {
                        throw std::runtime_error("Buffer overflow while reading UNSIGNED_INT bone IDs.");
                    }
                }
                else if (accessor.componentType == 5121) // UNSIGNED_BYTE
                {
                    if (srcOffset + sizeof(uint8_t) <= buffer.size())
                    {
                        uint8_t uint32Value = *reinterpret_cast<const uint8_t *>(&buffer[srcOffset]);
                        value = static_cast<int>(uint32Value);
                    }
                    else
                    {
                        throw std::runtime_error("Buffer overflow while reading UNSIGNED_BYTE bone IDs.");
                    }
                }

                attributeData[i * numComponents + j] = value;
                srcOffset += componentSize;
            }
            srcOffset += byteStride - componentSize * numComponents;
        }

        return attributeData;
    }

    std::vector<uint8_t> GLTFLoader::getBufferData(size_t accessorIndex)
    {
        const Accessor &accessor = accessors[accessorIndex];
        const BufferView &bufferView = bufferViews[accessor.bufferView];
        const std::vector<uint8_t> &buffer = buffersData[bufferView.buffer];
        size_t byteOffset = bufferView.byteOffset + accessor.byteOffset;
        size_t byteStride = bufferView.byteStride != 0 ? bufferView.byteStride : accessor.type.size() * sizeof(float);
        std::vector<uint8_t> data(accessor.count * byteStride);
        for (size_t i = 0; i < accessor.count; ++i)
        {
            size_t srcOffset = byteOffset + i * byteStride;
            size_t dstOffset = i * byteStride;
            if (srcOffset >= 0 && dstOffset >= 0 && srcOffset < buffer.size() && dstOffset < data.size() && byteStride <= buffer.size() - srcOffset && byteStride <= data.size() - dstOffset && buffer.data() && data.data())
            {
                std::memcpy(data.data() + dstOffset, buffer.data() + srcOffset, byteStride);
            }
        }
        return data;
    }

    std::vector<std::vector<size_t>> GLTFLoader::getBoneIDsData(size_t accessorIndex)
    {
        std::vector<std::vector<size_t>> boneIDsData;

        if (accessorIndex < accessors.size())
        {
            const std::vector<float> accessorData = getAttributeData(accessorIndex);

            size_t numComponents = 4; // Assumindo 4 componentes para bone IDs

            for (size_t i = 0; i < accessorData.size(); i += numComponents)
            {
                std::vector<size_t> boneIDs;

                for (size_t j = 0; j < numComponents; ++j)
                {
                    boneIDs.push_back(static_cast<size_t>(accessorData[i + j]));
                }

                boneIDsData.push_back(boneIDs);
            }
        }

        return boneIDsData;
    }

    std::vector<std::vector<float>> GLTFLoader::getWeightsData(size_t accessorIndex)
    {
        std::vector<std::vector<float>> weightsData;

        if (accessorIndex < accessors.size())
        {
            const std::vector<float> accessorData = getAttributeData(accessorIndex);

            size_t numComponents = 4; // Assumindo 4 componentes para pesos

            for (size_t i = 0; i < accessorData.size(); i += numComponents)
            {
                std::vector<float> weights;

                for (size_t j = 0; j < numComponents; ++j)
                {
                    weights.push_back(accessorData[i + j]);
                }

                weightsData.push_back(weights);
            }
        }

        return weightsData;
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

            if (meshJson.contains("extensions"))
            {
                mesh.extensions = meshJson["extensions"];
            }

            if (meshJson.contains("extras"))
            {
                mesh.extras = meshJson["extras"];
            }

            for (const auto &primitive : meshJson["primitives"])
            {

                // adicionar suporte bones

                SubMesh sm;

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
                        sm.positions.emplace_back(positionData[i], positionData[i + 1], positionData[i + 2]);
                    }
                }

                if (attributes.contains("COLOR_0"))
                {
                    size_t colorAccessorIndex = attributes["COLOR_0"].get<size_t>();
                    std::vector<float> colorData = getAttributeData(colorAccessorIndex);

                    for (size_t i = 0; i < colorData.size(); i += 4)
                    {
                        sm.colors.emplace_back(colorData[i], colorData[i + 1], colorData[i + 2], colorData[i + 3]);
                    }
                }
                else
                {
                    size_t positionAccessorIndex = attributes["POSITION"].get<size_t>();
                    std::vector<float> positionData = getAttributeData(positionAccessorIndex);
                    for (size_t i = 0; i < positionData.size() / 3; i++)
                    {
                        sm.colors.emplace_back(1, 1, 1, 1);
                    }
                }

                if (attributes.contains("NORMAL"))
                {
                    size_t normalAccessorIndex = attributes["NORMAL"].get<size_t>();
                    std::vector<float> normalData = getAttributeData(normalAccessorIndex);

                    for (size_t i = 0; i < normalData.size(); i += 3)
                    {
                        sm.normals.emplace_back(normalData[i], normalData[i + 1], normalData[i + 2]);
                    }
                }
                else
                {
                    size_t positionAccessorIndex = attributes["POSITION"].get<size_t>();
                    std::vector<float> positionData = getAttributeData(positionAccessorIndex);
                    for (size_t i = 0; i < positionData.size(); i += 3)
                    {
                        sm.normals.emplace_back(0, 0, 0);
                    }
                }

                if (attributes.contains("TEXCOORD_0"))
                {
                    size_t texcoordAccessorIndex = attributes["TEXCOORD_0"].get<size_t>();
                    std::vector<float> texcoordData = getAttributeData(texcoordAccessorIndex);

                    for (size_t i = 0; i < texcoordData.size(); i += 2)
                    {
                        sm.texcoords.emplace_back(texcoordData[i], texcoordData[i + 1]);
                    }
                }
                else
                {
                    size_t positionAccessorIndex = attributes["POSITION"].get<size_t>();
                    std::vector<float> positionData = getAttributeData(positionAccessorIndex);
                    for (size_t i = 0; i < positionData.size(); i += 3)
                    {
                        sm.texcoords.emplace_back(0, 0);
                    }
                }

                // Verifique se há dados BoneIDs e Weights
                if (attributes.contains("JOINTS_0") && attributes.contains("WEIGHTS_0"))
                {
                    size_t jointAccessorIndex = attributes["JOINTS_0"];
                    size_t weightAccessorIndex = attributes["WEIGHTS_0"];

                    // Obtenha os dados dos acessores de BoneIDs e Weights
                    sm.BoneIDs = getBoneIDsData(jointAccessorIndex);
                    sm.Weights = getWeightsData(weightAccessorIndex);
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
                        // std::vector<uint8_t> indices(indexBufferData.begin(), indexBufferData.end());
                        std::vector<uint8_t> indices;
                        const uint8_t *data = reinterpret_cast<const uint8_t *>(indexBufferData.data());
                        // size_t dataSize = indexBufferData.size() / sizeof(uint8_t);
                        size_t dataSize = indexAccessor.count * sizeof(uint8_t);
                        indices.assign(data, data + dataSize);

                        for (int i = 0; i < dataSize; i++)
                        {
                            sm.indices.push_back((unsigned int)indices[i]);
                        }

                        break;
                    }
                    case 5123:
                    { // UNSIGNED_SHORT

                        std::vector<uint16_t> indices;
                        const uint16_t *data = reinterpret_cast<const uint16_t *>(indexBufferData.data());
                        size_t dataSize = indexBufferData.size() / sizeof(uint16_t);
                        // size_t dataSize = indexAccessor.count * sizeof(uint16_t) * 2;
                        indices.assign(data, data + dataSize);

                        // indices.resize(dataSize);
                        for (int i = 0; i < dataSize; i++)
                        {
                            sm.indices.push_back((unsigned int)indices[i]);
                        }
                        break;
                    }
                    case 5125:
                    { // UNSIGNED_INT
                        std::vector<uint32_t> indices;
                        const uint32_t *data = reinterpret_cast<const uint32_t *>(indexBufferData.data());
                        // size_t dataSize = indexBufferData.size() / sizeof(uint32_t);
                        size_t dataSize = indexAccessor.count * sizeof(uint32_t) * 4;
                        indices.assign(data, data + dataSize);

                        for (int i = 0; i < dataSize; i++)
                        {
                            sm.indices.push_back((unsigned int)indices[i]);
                        }

                        break;
                    }
                    default:
                        throw std::runtime_error("Unsupported index component type.");
                    }

                    // corect mesh
                    vector<unsigned int> new_indice;
                    for (int i = 0; i < sm.indices.size() / 3; i += 3)
                    {

                        if ((sm.indices[i] < sm.indices.size() - 1 && sm.indices[i + 1] < sm.indices.size() - 1 && sm.indices[i + 2] < sm.indices.size() - 1))
                        {
                            new_indice.push_back(sm.indices[i]);
                            new_indice.push_back(sm.indices[i + 1]);
                            new_indice.push_back(sm.indices[i + 2]);
                        }
                        else
                        {
                            break;
                        }
                    }
                    sm.indices = new_indice;

                    if (primitive.contains("material"))
                    {
                        sm.material = primitive["material"].get<size_t>();
                    }
                }
                // break;
                mesh.sub_meshes.push_back(sm);
            }

            meshes.push_back(mesh);
        }

        return true;
    }

    bool GLTFLoader::assigneSkinsToMeshes()
    {
        for (Node n : nodes)
        {
            if (n.have_skin)
            {
                for (size_t st : n.meshIndices)
                {
                    meshes[st].have_skin = n.have_skin;
                    meshes[st].skin = n.skin;
                }
            }
        }
        return true;
    }

    bool GLTFLoader::load()
    {

        loadBuffers();

        loadBufferViews();

        loadAccessors();

        loadTextures();

        loadMeshes();

        loadScenes();

        loadLights();

        loadNodes();

        loadAnimations();

        loadMaterials();

        loadSkins();
        assigneSkinsToMeshes();


        return true;
    }
};