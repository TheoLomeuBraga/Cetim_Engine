#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <glm/vec3.hpp>



struct Entity {
    std::string name;
    std::map<std::string,std::string> info;
};

/*
struct Vertex {
    vec3 position;
    vec3 normal
    vec2 uv;
};

struct Mesh{
    std::vector<unsigned int> index;
    std::vector<Vertex> vertex;
};
*/
struct Brush_Part {
    glm::vec3 faces[3];
    std::string texture;
    glm::vec2 offset;
    float Rotation;
    glm::vec2 scale;
};
typedef std::vector<Brush_Part> Brush;

struct Structure {
    std::string name;
    Brush brush;
    std::map<std::string,std::string> info;
};

struct Full_Map_Info{
    std::vector<Entity> entitys;
    std::vector<Structure> structure;
};

bool parse_vertex(const std::string& line, glm::vec3& vertex) {
    std::istringstream iss(line);
    char paren;
    if (!(iss >> paren >> vertex.x >> vertex.y >> vertex.z >> paren)) {
        return false;
    }
    return true;
}

bool parse_key_value(const std::string& line, std::string& key, std::string& value) {
    std::istringstream iss(line);
    if (!(iss >> key >> value)) {
        return false;
    }
    return true;
}

bool parse_brush_part(const std::string& line, Brush_Part& brush_part) {
    std::istringstream iss(line);
    char paren;

    if (!(iss >> paren
            >> brush_part.faces[0].x >> brush_part.faces[0].y >> brush_part.faces[0].z >> paren
            >> paren
            >> brush_part.faces[1].x >> brush_part.faces[1].y >> brush_part.faces[1].z >> paren
            >> paren
            >> brush_part.faces[2].x >> brush_part.faces[2].y >> brush_part.faces[2].z >> paren
            >> brush_part.texture
            >> brush_part.offset.x >> brush_part.offset.y
            >> brush_part.Rotation
            >> brush_part.scale.x >> brush_part.scale.y)) {
        return false;
    }
    return true;
}

std::string remove_quotes(const std::string& input) {
    std::string output;
    for (char c : input) {
        if (c != '"' && c != '\'') {
            output.push_back(c);
        }
    }
    return output;
}

Full_Map_Info read_map_file(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        throw std::runtime_error("Error opening file");
    }

    std::string line;
    Full_Map_Info map_info;
    Entity current_entity;
    Structure current_structure;
    Brush_Part current_brush_part;
    bool in_entity = false;
    bool in_structure = false;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "//") {
            continue;
        } else if (token == "{") {
            if (in_entity) {
                in_structure = true;
                current_structure = Structure();
            } else {
                in_entity = true;
                current_entity = Entity();
            }
        } else if (token == "}") {
            if (in_structure) {
                in_structure = false;
                map_info.structure.push_back(current_structure);
            } else {
                in_entity = false;
                map_info.entitys.push_back(current_entity);
            }
        } else if (in_structure) {
            if (parse_brush_part(line, current_brush_part)) {
                current_structure.brush.push_back(current_brush_part);
            } else {
                std::string key, value;
                if (parse_key_value(line, key, value)) {
                    key = remove_quotes (key);
                    value = remove_quotes (value);
                    current_structure.info[key] = value;
                    if (key == "classname") {
                        current_structure.name = value;
                    }
                }
            }
        } else {
            std::string key, value;
            if (parse_key_value(line, key, value)) {
                key = remove_quotes (key);
                value = remove_quotes (value);
                current_entity.info[key] = value;
                if (key == "classname") {
                    current_entity.name = value;
                }
            }
        }
    }

    file.close();
    return map_info;
}

namespace print_read_map_file {
    void Print_Vec2(glm::vec2 v2){
        std::cout << v2.x;
        std::cout << " ";
        std::cout << v2.y;
        std::cout << std::endl;
    }
    void Print_Vec3(glm::vec3 v3){
        std::cout << v3.x;
        std::cout << " ";
        std::cout << v3.y;
        std::cout << " ";
        std::cout << v3.z;
        std::cout << std::endl;
    }

    void Print_Brush_Part(Brush_Part bp){
        Print_Vec3(bp.faces[0]);
        Print_Vec3(bp.faces[1]);
        Print_Vec3(bp.faces[2]);
        std::cout << bp.texture << std::endl;
        Print_Vec2(bp.offset);
        std::cout << bp.Rotation << std::endl;
        Print_Vec2(bp.scale);
    }
    void Print_structure(Structure s){
        std::cout << "structure name: " << s.name << std::endl;
        for(Brush_Part bp : s.brush){
            Print_Brush_Part(bp);
        }
        for(pair<std::string,std::string> i : s.info){
            std::cout << i.first;
            std::cout << " ";
            std::cout << i.second;
            std::cout << std::endl;
        }
    }
    void Print_Entity(Entity e){
        std::cout << "entity name:" << e.name << std::endl;
        for(pair<std::string,std::string> i : e.info){
            std::cout << i.first;
            std::cout << " ";
            std::cout << i.second;
            std::cout << std::endl;
        }
    }
    void Print_Full_Map_Info(Full_Map_Info fmi){
        for(Entity e : fmi.entitys){
            Print_Entity(e);
        }
        for(Structure s : fmi.structure){
            Print_structure(s);
        }
    }
};


void test_read_map_file() {
    std::string filename = "Leveis 3D/mesh_test.map"; // Replace with your .map filename
    Full_Map_Info map_info = read_map_file(filename);
    print_read_map_file::Print_Full_Map_Info(map_info);
}
