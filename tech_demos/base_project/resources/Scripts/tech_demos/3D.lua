require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_collision")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.objects.scene_3D")

local demo = {}

local demo = {
    map_data = {},
    map_objects = {},

    player_data = {},
    player_object = {}
}

local this_layers = {}

function create_player_part(layer,obj_data)
end
function create_player_ceane(layer,ceane_data)
end

function create_game_object(layer,obj_data)

    --demo.player_data = get_scene_3D("resources/Levels/3D/test_charter.gltf")
    --demo.player_object = create_player_ceane(layers.cenary,demo.map_data)

end
function create_ceane(layer,ceane_data)

end

function demo:START(layers)

    this_layers = deepcopy(layers)

    demo.map_data = get_scene_3D("resources/Levels/3D/test_level.gltf")
    demo.map_objects = create_ceane(layers.cenary,demo.map_data)

    
    
end

function demo:UPDATE()

end

function demo:END()
    --remove_object(demo.map_objects.object_ptr)
    --remove_object(demo.player_object.object_ptr)
    clear_memory()
end

return demo