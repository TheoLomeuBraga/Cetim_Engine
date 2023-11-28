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
    
    new_asset_list_element(asset_types.image,"resources/Textures/loading.png"),
    new_asset_list_element(asset_types.image,"resources/Textures/null.png"),
    new_asset_list_element(asset_types.image,"resources/Textures/null.svg"),
    new_asset_list_element(asset_types.image,"resources/Levels/3D/hub/null.png"),
    new_asset_list_element(asset_types.image,"resources/Textures/arm_cannon_texture.png"),
    
    new_asset_list_element(asset_types.scene_3D,"resources/3D Models/bullets.gltf"),
    new_asset_list_element(asset_types.scene_3D,"resources/3D Models/arm_cannon.gltf"),
    new_asset_list_element(asset_types.scene_3D,"resources/Levels/3D/hub/hub.gltf"),

    new_asset_list_element(asset_types.audio,"resources/Audio/sounds/jump.wav"),
    new_asset_list_element(asset_types.audio,"resources/Audio/sounds/shot_1.wav"),
    new_asset_list_element(asset_types.audio,"resources/Audio/sounds/shot_2.wav"),
    new_asset_list_element(asset_types.audio,"resources/Audio/sounds/shot_3.wav"),

    new_asset_list_element(asset_types.script,"game_scripts/player/charter_arcenal"),
    new_asset_list_element(asset_types.script,"game_scripts/player/charter_data"),
    new_asset_list_element(asset_types.script,"game_scripts/player/charter_interaction"),
    new_asset_list_element(asset_types.script,"game_scripts/player/charter_movement"),

    

    new_asset_list_element(asset_types.script,"game_scripts/mensage"),
    new_asset_list_element(asset_types.script,"game_scripts/door_triger"),
    new_asset_list_element(asset_types.script,"game_scripts/item"),
    
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
    
    --print("stop_coroutine_if_is_not_loaded start")

    stop_coroutine_if_is_not_loaded(assets_needed)

    --print("stop_coroutine_if_is_not_loaded end")

    --print("CCCCC")

    coroutine.yield()
    level.scene_3D_data = get_scene_3D("resources/Levels/3D/hub/hub.gltf")

    

    coroutine.yield()

    --print("DDDDD")
    cenary_builders.cenary = create_object(layers.cenary)

    coroutine.yield()
    
    scene_data = cenary_builders.scene(cenary_builders.cenary,2,level.scene_3D_data,true)

    --print("FFFFF")

    coroutine.yield()
    

    --print("GGGGG")
    
    

    camera = {}

    global_data:set_var("level_animation_data",{path="resources/Levels/3D/hub/hub.gltf",parts_ptr_list=scene_data.parts_ptr_list})

    loading_screen.close()

    time:set_speed(1)
    
end




local time_passed = 0
function level:UPDATE()

    
    --[[
    if scene_data ~= nil then
        if time_passed < 0 then
            time_passed = 1
        end
        time:get()
        time_passed = time_passed - time.delta * time.scale * 0.1
        set_keyframe("resources/Levels/3D/hub/hub.gltf", scene_data.parts_ptr_list, true, "open_door_A",time_passed)
    end
    ]]
    
    

end

function level:END()
    remove_object(cenary_builders.cenary)
    coroutine.yield()
    global_data:set_var("level_animation_data",{})
    clear_memory()
    coroutine.yield()
end

return level
