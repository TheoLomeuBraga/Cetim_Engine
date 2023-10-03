#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <any>


#define entity size_t

struct ecs_system
{

    void (*add)(entity, std::any);
    bool (*have)(entity);

    void (*run)(entity);
    void (*run_components)();

    void (*remove)(entity);
};

std::unordered_map<std::string, struct ecs_system> ecs_systems_registerd;

std::set<entity> entityes_set = {},free_entityes = {};
entity next_entity_id = 1;

entity find_free_entity()
{
    entity i = 1;
    while (i < SIZE_MAX)
    {
        if (entityes_set.find(i) == entityes_set.end())
        {
            return i;
        }
        i++;
    }
    return 0;
}

entity new_entity()
{
    if (!free_entityes.empty())
    {
        entity id = *free_entityes.begin();
        free_entityes.erase(id);
        return id;
    }
    else
    {
        return next_entity_id++;
    }
}

void add_component(entity entity, std::string component_name, std::any arg)
{
    ecs_systems_registerd[component_name].add(entity, arg);
}

bool have_component(entity entity, std::string component_name)
{
    if (ecs_systems_registerd[component_name].have(entity))
    {
        return true;
    }
    return false;
}

void remove_component(entity entity, std::string component_name)
{
    ecs_systems_registerd[component_name].remove(entity);
}

void run_entity(entity entity)
{
    for (std::pair<std::string, struct ecs_system> p : ecs_systems_registerd)
    {
        if (p.second.have(entity))
        {
            p.second.run(entity);
        }
    }
}

void run_components()
{
    for (std::pair<std::string, struct ecs_system> p : ecs_systems_registerd)
    {
        p.second.run_components();
    }
}

void delete_entity(entity entity)
{
    free_entityes.insert(entity);
    for (std::pair<std::string, struct ecs_system> p : ecs_systems_registerd)
    {
        if (p.second.have(entity))
        {
            p.second.remove(entity);
        }
    }
    entityes_set.erase(entity);
}

void clean_components()
{
    for(entity i : entityes_set){
        delete_entity(i);
    }
}