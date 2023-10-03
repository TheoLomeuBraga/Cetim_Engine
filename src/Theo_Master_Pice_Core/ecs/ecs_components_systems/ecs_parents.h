#pragma once 

#include "ecs/ecs.h"
#include <vector>
#include <string>
#include <map>
#include <set>



struct family {
    entity father;
    std::set<entity> children;
};


std::map<entity,struct family> familys_map;

void add_family(entity id,std::any data){
    familys_map.insert(std::pair<entity,struct family>(id,std::any_cast<struct family>(data)));
}

bool have_family(entity id){
    if(familys_map.find(id) != familys_map.end()){
        return true;
    }
    return false;
}

void run_family(entity id){}

void run_familys(){}

void remove_family (entity id){
    familys_map.erase(id);
}

void register_family_component(){
    ecs_systems_registerd.insert(std::pair<std::string,struct ecs_system>("family",{add_family,have_family,run_family,run_familys,remove_family}));
}
