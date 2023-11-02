require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_render_shader")

triger_target = ""
open_progres = 0

level_animation_data = {}
function START()
    
end

function UPDATE()

    time:get()
    if level_animation_data.path == nil then
        level_animation_data = global_data:get_var("level_animation_data")
        if level_animation_data == nil then
            level_animation_data = {}
        end
    else
        set_keyframe(level_animation_data.path, level_animation_data.parts_ptr_list, true, "open_door_A",open_progres)
    end
    
    
    open_progres = open_progres + time.delta
    if open_progres > 1 then
        open_progres = 1
    end
    
end

function END()
end
