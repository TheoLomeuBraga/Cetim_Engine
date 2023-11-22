require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_render_shader")

this_object = nil

rotate = true
local rotation = 0

function START()
    this_object = game_object:new(this_object_ptr)
end

local is_player_touch = function(objs_touching)
    for key, value in pairs(objs_touching) do
        local obj_touching = game_object:new(value)
        if obj_touching.components[components.lua_scripts]:has_script("game_scripts/player/charter_data") then
            return true
        end
    end
    return false
end

function UPDATE()
    local player_touch = is_player_touch(this_object.components[components.physics_3D]:get_objects_coliding())
    
    if player_touch then
        remove_object(this_object_ptr)
    end

    if rotate then
        time:get()

        this_object.components[components.transform]:change_rotation(0,rotation,0)
        rotation = rotation + (time.delta * time.scale * 180)
    end
end

function COLLIDE(collision_info)
end

function END()
end
