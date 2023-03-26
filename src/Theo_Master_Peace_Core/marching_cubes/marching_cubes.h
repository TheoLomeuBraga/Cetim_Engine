#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <map>

#include "marching_cubes/marching_cubes_tables.h"

namespace marching_cubes
{

    struct vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texcoord;
    };

    struct Mesh
    {
        std::vector<vertex> vertces;
        std::vector<unsigned int> indices;
    };

    struct MarchingCubesTerrain
    {
        int width;               // Width of the terrain grid
        int height;              // Height of the terrain grid
        int depth;               // Depth of the terrain grid
        float isoLevel;          // Iso-surface threshold value
        std::vector<float> grid; // 3D grid of scalar values
        std::vector<unsigned char> typesGrid;

        MarchingCubesTerrain(int width, int height, int depth, float isoLevel)
            : width(width), height(height), depth(depth), isoLevel(isoLevel), grid(width * height * depth), typesGrid(width * height * depth)
        {
        }

        // Get the density value at grid point (x, y, z)
        float getDensity(int x, int y, int z) const
        {
            return grid[x + y * width + z * width * height];
        }

        // Set the density value at grid point (x, y, z)
        void setDensity(int x, int y, int z, float density)
        {
            grid[x + y * width + z * width * height] = density;
        }

        unsigned char getType(int x, int y, int z) const
        {
            return typesGrid[x + y * width + z * width * height];
        }

        // Set the density value at grid point (x, y, z)
        void setType(int x, int y, int z, unsigned char type)
        {
            typesGrid[x + y * width + z * width * height] = type;
        }
    };

    glm::vec3 interpolateVertex(float isoLevel, const glm::vec3 &p1, const glm::vec3 &p2, float valP1, float valP2)
    {
        float t = (isoLevel - valP1) / (valP2 - valP1);
        return p1 + t * (p2 - p1);
    }

    glm::vec3 computeNormal(const MarchingCubesTerrain &terrain, const glm::vec3 &position)
    {
        glm::vec3 normal(
            terrain.getDensity(position.x + 1, position.y, position.z) - terrain.getDensity(position.x - 1, position.y, position.z),
            terrain.getDensity(position.x, position.y + 1, position.z) - terrain.getDensity(position.x, position.y - 1, position.z),
            terrain.getDensity(position.x, position.y, position.z + 1) - terrain.getDensity(position.x, position.y, position.z - 1));

        return glm::normalize(normal);
    }

    glm::vec2 computeTexcoord(const glm::vec3 &position, const MarchingCubesTerrain &terrain)
    {
        return glm::vec2(position.x / terrain.width, position.y / terrain.height);
    }

    Mesh marchingCubesToMesh(const MarchingCubesTerrain &terrain)
    {
        Mesh mesh;

        for (int x = 0; x < terrain.width - 1; ++x)
        {
            for (int y = 0; y < terrain.height - 1; ++y)
            {
                for (int z = 0; z < terrain.depth - 1; ++z)
                {
                    glm::ivec3 cube[8] = {
                        {x, y, z},
                        {x + 1, y, z},
                        {x + 1, y + 1, z},
                        {x, y + 1, z},
                        {x, y, z + 1},
                        {x + 1, y, z + 1},
                        {x + 1, y + 1, z + 1},
                        {x, y + 1, z + 1}};

                    int cubeIndex = 0;
                    for (int i = 0; i < 8; ++i)
                    {
                        if (terrain.getDensity(cube[i].x, cube[i].y, cube[i].z) < terrain.isoLevel)
                        {
                            cubeIndex |= 1 << i;
                        }
                    }

                    if (edgeTable[cubeIndex] == 0)
                    {
                        continue;
                    }

                    glm::vec3 vertList[12];
                    if (edgeTable[cubeIndex] & 1)
                    {
                        vertList[0] = interpolateVertex(terrain.isoLevel, glm::vec3(cube[0]), glm::vec3(cube[1]),
                                                        terrain.getDensity(cube[0].x, cube[0].y, cube[0].z),
                                                        terrain.getDensity(cube[1].x, cube[1].y, cube[1].z));
                    }
                    // Interpolate other vertex positions similarly...

                    for (int i = 0; triTable[cubeIndex][i] != -1; i += 3)
                    {
                        for (int j = 0; j < 3; ++j)
                        {
                            int edgeIndex = triTable[cubeIndex][i + j];
                            glm::vec3 position = vertList[edgeIndex];

                            // Compute the normal and texcoord using helper functions
                            glm::vec3 normal = computeNormal(terrain, position);
                            glm::vec2 texcoord = computeTexcoord(position, terrain);

                            vertex vert = {position, normal, texcoord};
                            mesh.vertces.push_back(vert);
                            mesh.indices.push_back(static_cast<unsigned int>(mesh.vertces.size()) - 1);
                        }
                    }
                }
            }
        }

        return mesh;
    }

    std::map<unsigned int, Mesh> marchingCubesToMeshWithTypes(const MarchingCubesTerrain &terrain)
    {
        std::map<unsigned int, Mesh> meshes;

        // Iterate through the cells in the terrain grid
        for (int z = 0; z < terrain.depth - 1; ++z)
        {
            for (int y = 0; y < terrain.height - 1; ++y)
            {
                for (int x = 0; x < terrain.width - 1; ++x)
                {
                    // Calculate the cube index and perform marching cubes algorithm
                    unsigned int cubeIndex = 0;
                    glm::vec3 vertList[12];

                    // Code for calculating the cube index and vertList
                    // ...

                    unsigned char type = terrain.getType(x, y, z);

                    // Generate the mesh vertices and indices based on the cubeIndex and type
                    for (int i = 0; triTable[cubeIndex][i] != -1; i += 3)
                    {
                        vertex v1, v2, v3;
                        v1.position = vertList[triTable[cubeIndex][i]];
                        v2.position = vertList[triTable[cubeIndex][i + 1]];
                        v3.position = vertList[triTable[cubeIndex][i + 2]];

                        v1.normal = computeNormal(terrain, v1.position);
                        v2.normal = computeNormal(terrain, v2.position);
                        v3.normal = computeNormal(terrain, v3.position);

                        v1.texcoord = computeTexcoord(v1.position, terrain);
                        v2.texcoord = computeTexcoord(v2.position, terrain);
                        v3.texcoord = computeTexcoord(v3.position, terrain);

                        unsigned int index = meshes[type].vertces.size();
                        meshes[type].vertces.push_back(v1);
                        meshes[type].vertces.push_back(v2);
                        meshes[type].vertces.push_back(v3);

                        meshes[type].indices.push_back(index);
                        meshes[type].indices.push_back(index + 1);
                        meshes[type].indices.push_back(index + 2);
                    }
                }
            }
        }

        return meshes;
    }

};