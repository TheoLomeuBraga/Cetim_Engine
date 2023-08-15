require("engine_libs.short_cuts.create_mesh")
require("engine_libs.short_cuts.create_collision")
require("engine_libs.short_cuts.create_sound")
require("engine_libs.objects.time")
require("engine_libs.objects.scene_3D")
require("resources.playable_scene")

local level = {
    camera_obj = {},
    cenary_obj = {},
    player_obj = {},
    scene_3D_data = {},
}

local assets_needed = {
    new_asset_list_element(asset_types.scene_3D,"resources/Levels/3D/test_map.gltf")
}

function level:START(layers)

    level.camera_obj = create_camera_perspective(layers.camera, { x = 0, y = 0, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    set_lisener_object(level.camera_obj.object_ptr)

    stop_coroutine_if_is_not_loaded(assets_needed,true)

    level.scene_3D_data = get_scene_3D("resources/Levels/3D/test_map.gltf")
    cenary_builders.cenary_obj = cenary_builders.scene(layers.cenary,level.scene_3D_data)
    
end





function level:UPDATE()

    
    

end

function level:END()
    clear_memory()
end

return level
