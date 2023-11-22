require("objects.game_object")
require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")



passage_target = ""

local enable = true

function START()
end

function UPDATE()
end

function COLLIDE(collision_info)
    if enable then
        local obj_colide = game_object:new(collision_info.collision_object)
        if obj_colide.components ~= nil and obj_colide.components[components.lua_scripts] ~= nil and obj_colide.components[components.lua_scripts]:has_script("game_scripts/player/charter_movement") then
            local core_obj = game_object:new(global_data:get_var("core_object_ptr"))
            core_obj.components[components.lua_scripts]:call_function("core", "load_sceane", { passage_target })
            enable = false
        end
    end
end

function END()
end
