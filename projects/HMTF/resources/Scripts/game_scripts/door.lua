require("definitions")
require("components.extras")
require("components.component_all")
require("components.component_index")

require("objects.game_object")
require("objects.time")
require("objects.material")
require("objects.global_data")
require("objects.vectors")
require("objects.input")
require("objects.scene_3D")
require("resources.playable_scene")
require("objects.global_data")

require("math")

door_sceane_path = "resources/3D Models/basic_door.gltf"
door_sceane_brute_data = {}

door_sceane_data = {}

layers = {}

function START()

    layers = global_data:get_var("layers")

    door_sceane_brute_data = get_scene_3D(door_sceane_path)
    cenary_builders.scene(this_object_ptr,2,door_sceane_brute_data,false)

end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end
