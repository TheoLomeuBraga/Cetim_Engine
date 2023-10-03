#pragma once 

#include "ecs.h"
#include <vector>
#include <string>
#include <map>
#include <set>

std::map<entity,std::string> names_map;

void add_name(entity id,std::any data){
    names_map.insert(std::pair<entity,std::string>(id,std::any_cast<std::string>(data)));
}

bool have_name(entity id){
    if(names_map.find(id) != names_map.end()){
        return true;
    }
    return false;
}

void run_name(entity id){}

void run_names(){}

void remove_name (entity id){
    names_map.erase(id);
}

void register_name_component(){
    ecs_systems_registerd.insert(std::pair<std::string,struct ecs_system>("name",{add_name,have_name,run_name,run_names,remove_name}));
}

std::string* get_name(entity id){
    return &names_map[id];
}

entity search_entity_by_name(std::string name){
    for(std::pair<entity,std::string> p : names_map){
        if(p.second == name){
            return p.first;
        }
    }
    return 0;
}
