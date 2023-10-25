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
    
}

scene_data = nil

function level:START()

    --print("AAAAA")
    coroutine.yield()

    local layers = global_data:get_var("layers")
    
    time:set_speed(0)

    --print("BBBBB")
    coroutine.yield()

    loading_screen.open()

    coroutine.yield()
    --coroutine.yield()

    stop_coroutine_if_is_not_loaded(assets_needed)

    coroutine.yield()
    level.scene_3D_data = get_scene_3D("resources/Levels/3D/hub/hub.gltf")

    coroutine.yield()

    cenary_builders.cenary = create_object(layers.cenary)

    coroutine.yield()
    
    scene_data = cenary_builders.scene(cenary_builders.cenary,2,level.scene_3D_data,true)
    
    coroutine.yield()
    time:set_speed(1)
    
    loading_screen.close()

    camera = {}
    
end





function level:UPDATE()

    
    if scene_data ~= nil then
        set_keyframe("resources/Levels/3D/hub/hub.gltf", scene_data.parts_ptr_list, false, "open_door_A",0.65)
    end
    
    

end

function level:END()
    remove_object(cenary_builders.cenary)
    coroutine.yield()
    clear_memory()
    coroutine.yield()
end

return level
