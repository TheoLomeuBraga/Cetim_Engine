require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_render_shader")

map_file = ""
parts_ptr_list = {}
doors_open = {}
doors_open_progress = {}

function START()
    if map_file ~= "" then
        local scene = get_scene_3D(map_file)
        for key, value in pairs(scene.animations) do
            doors_open[key] = false
            doors_open_progress[key] = 0
        end
    end
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end
