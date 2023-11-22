require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_render_shader")

this_object = nil

function START()
    this_object = game_object:new(this_object_ptr)
    this_object.components[components.physics_3D].get_collision_info = true
    this_object.components[components.physics_3D]:set()
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end
