require("definitions")
require("components.extras")
require("components.component_index")
require("components.component_all")
require("objects.game_object")
require("objects.global_data")

local core_obj = nil

function summon_entity(pos, rot_y, type)
    if core_obj == nil then
        core_obj = game_object(global_data.core_object_ptr)
    end

    
    core_obj.components.lua_scripts.scripts["game_scripts/managers/entity_master"].functions.summon_entity({ pos = pos, rot_y = rot_y, type = type })
end

function check_hit(ptr,data)
    if global_data.core_object_ptr ~= nil then
        if core_obj == nil then
            core_obj = game_object(global_data.core_object_ptr)
        end

        core_obj.components.lua_scripts.scripts["game_scripts/managers/entity_master"].functions.check_hit({ptr,data})
    end
end

function clean_entityes()
    if global_data.core_object_ptr ~= nil then
        if core_obj == nil then
            core_obj = game_object(global_data.core_object_ptr)
        end

        core_obj.components.lua_scripts.scripts["game_scripts/managers/entity_master"].functions.clean({})
    end
end

