require("engine_libs.objects.game_object")
require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")
require("engine_libs.objects.global_data")



passage_target = ""

function START()
end

function UPDATE()
end

function COLLIDE(collision_info)

    local obj_colide = game_object:new(collision_info.collision_object)

    if obj_colide.components ~= nil and obj_colide.components[components.lua_scripts] ~= nil and obj_colide.components[components.lua_scripts]:has_script("game_scripts/hmtf_charter") then

        local core_obj = game_object:new(global_data:get_var("core_object_ptr"))
        core_obj.components[components.lua_scripts]:call_function("core", "load_sceane", {passage_target})

    end
    
end

function END()
end
