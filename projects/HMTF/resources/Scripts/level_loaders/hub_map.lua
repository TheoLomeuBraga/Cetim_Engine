require("short_cuts.create_mesh")
require("short_cuts.create_collision")
require("objects.time")
require("objects.game_object")
require("components.physics_3D")
require("objects.scene_3D")
require("components.audio_source")
require("resources.playable_scene")

local level = {
    cenary_obj = {},
    entities_obj = {},
    scene_3D_data = {},
}

core_obj = {}

local assets_needed = {
    new_asset_list_element(asset_types.scene_3D,"resources/Levels/3D/hub/hub.gltf"),
    new_asset_list_element(asset_types.image,"resources/Textures/loading.png"),
    new_asset_list_element(asset_types.scene_3D,"resources/3D Models/arm_cannon.gltf"),
    new_asset_list_element(asset_types.image,"resources/Textures/arm_cannon_texture.png"),
    new_asset_list_element(asset_types.scene_3D,"resources/3D Models/bullets.gltf"),
    new_asset_list_element(asset_types.scene_3D,"resources/3D Models/basic_door.gltf"),
    
}

function level:START()

    --print("AAAAA")
    coroutine.yield()

    local layers = global_data:get_var("layers")
    
    time:set_speed(0)

    --print("BBBBB")
    coroutine.yield()

    core_obj = game_object:new(global_data:get_var("core_object_ptr"))
    core_obj.components[components.lua_scripts]:call_function("core","set_load_image",{path = "resources/Textures/loading.png",color={r=1,g=1,b=1}})

    local camera = create_camera_perspective(layers.camera, { x = 0, y = 0, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)

    coroutine.yield()

    set_lisener_object(camera.object_ptr)

    coroutine.yield()
    --coroutine.yield()

    stop_coroutine_if_is_not_loaded(assets_needed)

    coroutine.yield()
    level.scene_3D_data = get_scene_3D("resources/Levels/3D/hub/hub.gltf")

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
    coroutine.yield()
    clear_memory()
    coroutine.yield()
end

return level
