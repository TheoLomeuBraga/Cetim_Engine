require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_render_shader")

triger_target = ""
open_speed = 2
open_progres = 0

level_animation_data = {}

this_physics_3d = {}


function START()
    this_physics_3d = game_object:new(this_object_ptr).components[components.physics_3D]
    this_physics_3d.get_collision_info = true
    this_physics_3d:set()
end

function get_valid_touches()
    for key, value in pairs(this_physics_3d.objs_touching) do
        local obj = game_object:new(value)
        if obj.components[components.lua_scripts] ~= nil and obj.components[components.lua_scripts]:has_script("game_scripts/charter") then
            return true
        end
    end
    return false
end

touch_player = false

function UPDATE()
    this_physics_3d:get()

    touch_player = get_valid_touches()

    time:get()
    if level_animation_data.path == nil then
        level_animation_data = global_data:get_var("level_animation_data")
        if level_animation_data == nil then
            level_animation_data = {}
        end
    else
        set_keyframe(level_animation_data.path, level_animation_data.parts_ptr_list, true, "open_door_A", open_progres)

        
        set_keyframe(level_animation_data.path, level_animation_data.parts_ptr_list, true, "A", 0.5)
    end



    if touch_player then
        open_progres = open_progres + (time.delta * open_speed)
        if open_progres > 1 then
            open_progres = 1
        end
    else
        open_progres = open_progres - (time.delta * open_speed)
        if open_progres < 0 then
            open_progres = 0
        end
    end
end

function COLLIDE(collision_info)
    deepprint(collision_info)
end

function END()
end
