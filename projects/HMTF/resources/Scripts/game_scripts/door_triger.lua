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
update_open_progres = false

level_animation_data = {}

this_physics_3d = {}


function START()
    this_physics_3d = game_object:new(this_object_ptr).components[components.physics_3D]
    this_physics_3d.get_collision_info = true
    this_physics_3d:set()

    --make_banchmark(this_object_ptr,"game_scripts/door_triger")
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

start_base_animation = true
touch_player = false

function UPDATE()

    

    time:get()

    
    
    this_physics_3d:get()
    
    

    touch_player = get_valid_touches()
    
    
    
    if level_animation_data.path == nil then
        level_animation_data = global_data:get_var("level_animation_data")
        if level_animation_data == nil then
            level_animation_data = {}
        end
    else
        if start_base_animation then
            set_keyframe(level_animation_data.path, level_animation_data.parts_ptr_list, false, "A", 0)
            set_keyframe(level_animation_data.path, level_animation_data.parts_ptr_list, false, "B", 0)
            start_base_animation = false
        end
        if update_open_progres then
            set_keyframe(level_animation_data.path, level_animation_data.parts_ptr_list, true, "open_door_A",open_progres)
            set_keyframe(level_animation_data.path, level_animation_data.parts_ptr_list, true, "A", open_progres)
            set_keyframe(level_animation_data.path, level_animation_data.parts_ptr_list, true, "B", open_progres)
            update_open_progres = false
        end
    end

    

    update_open_progres = true
    if touch_player then
        
        open_progres = open_progres + (time.delta * time.sacale * open_speed)
        if open_progres > 1 then
            open_progres = 1
            update_open_progres = false
        end
    else
        open_progres = open_progres - (time.delta * time.sacale * open_speed)
        
        if open_progres < 0 then
            open_progres = 0
            update_open_progres = false
        end
    end

end

function COLLIDE(collision_info)
    deepprint(collision_info)
end

function END()
end
