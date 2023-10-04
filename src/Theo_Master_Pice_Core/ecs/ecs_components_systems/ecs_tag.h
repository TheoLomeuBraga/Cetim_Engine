#pragma once

#include "../ecs.h"
#include <vector>
#include <string>
#include <map>
#include <set>

std::map<entity, std::set<std::string>> tags_map;

void add_tag(entity id, std::any data)
{
    tags_map.insert(std::pair<entity, std::set<std::string>>(id, std::any_cast<std::set<std::string>>(data)));
}

bool have_tag(entity id)
{
    if (tags_map.find(id) != tags_map.end())
    {
        return true;
    }
    return false;
}

void run_tag(entity id) {}

void run_tags() {}

void remove_tag(entity id)
{
    tags_map.erase(id);
}

void register_tag_component()
{
    ecs_systems_registerd.insert(std::pair<std::string, struct ecs_system>("tag", {add_tag, have_tag, run_tag, run_tags, remove_tag}));
}

entity search_entity_by_tag(std::string tag)
{
    for (std::pair<entity, std::set<std::string>> p : tags_map)
    {
        for (std::string s : p.second)
        {
            if (s == tag)
            {
                return p.first;
            }
        }
    }
    return 0;
}