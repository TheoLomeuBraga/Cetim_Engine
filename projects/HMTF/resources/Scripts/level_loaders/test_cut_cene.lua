require("short_cuts.create_mesh")
require("short_cuts.create_collision")
require("objects.time")
require("objects.scene_3D")
require("custom_libs.playable_scene")

local level = {
    cenary_obj = {},
    entities_obj = {},
    scene_3D_data = {},
}

core_obj = {}

speachs = {
    "1",
    "22",
    "333",
    "4444",
    "55555",
}

local assets_needed = {
    --new_asset_list_element(asset_types.scene_3D,"resources/Levels/3D/test_map.gltf")
    new_asset_list_element(asset_types.scene_3D,"resources/Levels/3D/test_cut_cene.gltf")
    
}

function level:START()

    print("test_cut_cene")

    

    local layers = global_data:get_var("layers")
    
    time:set_speed(0)

    core_obj = game_object(global_data:get_var("core_object_ptr"))
    core_obj.components[components.lua_scripts]:call_function("core","set_load_image",{path = "resources/Textures/loading.png",color={r=1,g=1,b=1}})

    

    local camera = create_camera_perspective(layers.camera, { x = 0, y = 0, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    set_lisener_object(camera.object_ptr)
    

    stop_coroutine_if_is_not_loaded(assets_needed)

    level.scene_3D_data = get_scene_3D("resources/Levels/3D/test_cut_cene.gltf")

    coroutine.yield()

    cenary_builders.cenary = create_object(layers.cenary)
    cenary_builders.scene(cenary_builders.cenary,2,level.scene_3D_data,true)

    coroutine.yield()

    core_obj.components[components.lua_scripts]:call_function("core","set_load_image",{})

    coroutine.yield()
    
    time:set_speed(1)
    remove_object(camera.object_ptr)
    camera = {}
    
end





function level:UPDATE()

    
    

end

function level:END()
    remove_object(cenary_builders.cenary)
    clear_memory()
end

return level
