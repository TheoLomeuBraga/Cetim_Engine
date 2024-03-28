require("definitions")
require("components.extras")
require("components.component_index")

require("components.component_all")

require("objects.game_object")

require("objects.global_data")

local core_obj = nil

function summon_bullet(pos,direction,mesh, spred, speed, life_time, damage, quantity, hit_scan,base_impulse,target,friendly,color,behavior)
    
    if core_obj == nil then
        core_obj = game_object(global_data.core_object_ptr)
    end
    local args = {pos = pos,direction=direction,mesh = mesh, spred = spred, speed = speed, life_time = life_time, damage = damage, quantity = quantity,hit_scan = hit_scan,base_impulse = base_impulse,target = target,friendly=friendly,color = color,behavior = behavior}
    core_obj.components.lua_scripts:call_function("game_scripts/bullet_master","summon_bullet",args)
end

function summon_bullet(pos,direction,mesh, spred, speed, life_time, damage, quantity, hit_scan,base_impulse,target,friendly,color,behavior)
    
    if core_obj == nil then
        core_obj = game_object(global_data.core_object_ptr)
    end
    local args = {pos = pos,direction=direction,mesh = mesh, spred = spred, speed = speed, life_time = life_time, damage = damage, quantity = quantity,hit_scan = hit_scan,base_impulse = base_impulse,target = target,friendly=friendly,color = color,behavior = behavior}
    core_obj.components.lua_scripts:call_function("game_scripts/bullet_master","summon_bullet",args)
end

function clean_bullets()
    if global_data.core_object_ptr ~= nil then
        print("A")
        if core_obj == nil then
            core_obj = game_object(global_data.core_object_ptr)
        end
        print("B")

        core_obj.components.lua_scripts.scripts["game_scripts/bullet_master"].functions.clean({})
        print("C")
    end
end