require("short_cuts.create_mesh")
require("short_cuts.create_collision")
require("objects.time")
require("objects.game_object")
require("components.physics_3D")
require("objects.scene_3D")
require("components.audio_source")
require("components.ui_component")
require("game_scripts.resources.playable_scene")
require("function_sets.navmesh")
require("game_scripts.resources.entity_api")
require("game_scripts.resources.bullet_api")

local level = {
    cenary_obj = {},
    entities_obj = {},
    scene_3D_data = {},
}

core_obj = {}

local assets_needed = {
    
    new_asset_list_element(asset_types.image,"Textures/loading.png"),
    new_asset_list_element(asset_types.image,"Textures/null.png"),
    new_asset_list_element(asset_types.image,"Textures/null.svg"),
    new_asset_list_element(asset_types.image,"Levels/hub/null.png"),
    new_asset_list_element(asset_types.image,"Textures/arm_cannon_texture.png"),
    new_asset_list_element(asset_types.image,"Textures/energy_buble.png"),
    
    new_asset_list_element(asset_types.scene_3D,"3D Models/bullets.gltf"),
    new_asset_list_element(asset_types.scene_3D,"3D Models/arm_cannon.glb"),
    new_asset_list_element(asset_types.scene_3D,"Levels/floating_island_hub/floating_island_hub.glb"),

    new_asset_list_element(asset_types.audio,"Audio/sounds/jump.wav"),
    new_asset_list_element(asset_types.audio,"Audio/sounds/shot_1.wav"),
    new_asset_list_element(asset_types.audio,"Audio/sounds/shot_2.wav"),
    new_asset_list_element(asset_types.audio,"Audio/sounds/shot_3.wav"),

    new_asset_list_element(asset_types.script,"game_scripts/player/charter_data"),
    new_asset_list_element(asset_types.script,"game_scripts/player/charter_interaction"),
    new_asset_list_element(asset_types.script,"game_scripts/player/charter_movement"),

    new_asset_list_element(asset_types.script,"game_scripts/mensage"),
    new_asset_list_element(asset_types.script,"game_scripts/door_triger"),
    new_asset_list_element(asset_types.script,"game_scripts/item"),
    
}

scene_data = nil

function button_func(id,state)
    print("lua",id,state)
    
end

function level:START()

    global_data.pause = 1

    global_data.player_object_ptr = nil

    coroutine.yield()

    clean_entityes()

    coroutine.yield()

    remove_all_navmesh()

    coroutine.yield()

    clean_bullets()

    local layers = global_data.layers
    
    time:set_speed(0)

    --print("BBBBB")
    coroutine.yield()

    loading_screen.open()

    coroutine.yield()

    stop_coroutine_if_is_not_loaded(assets_needed)

    coroutine.yield()

    --level.scene_3D_data = get_scene_3D("Levels/hub/hub.gltf")
    level.scene_3D_data = get_scene_3D("Levels/floating_island_hub/floating_island_hub.glb")
    

    coroutine.yield()
    cenary_builders.cenary = create_object(layers.cenary)

    coroutine.yield()
    
    scene_data = cenary_builders.scene(cenary_builders.cenary,2,level.scene_3D_data,true)

    coroutine.yield()
    
    game_object:new(cenary_builders.cenary).components.lua_scripts:add_script("game_scripts/navegation_test")
    

    camera = {}

    --global_data.level_animation_data = {path="Levels/hub/hub.gltf",parts_ptr_list=scene_data.parts_ptr_list}
    global_data.level_animation_data = {path="Levels/floating_island_hub/floating_island_hub.glb",parts_ptr_list=scene_data.parts_ptr_list}

    loading_screen.close()

    time:set_speed(1)

    global_data.pause = 0

    game_object(global_data.core_object_ptr).components.lua_scripts.scripts["core"].functions.set_background_image({ path = "Textures/test_sb.svg", color = { r = 1, g = 1, b = 1 } })
    
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
        set_keyframe("Levels/hub/hub.gltf", scene_data.parts_ptr_list, true, "open_door_A",time_passed)
    end
    ]]
    
    

end

function level:END()
    remove_object(cenary_builders.cenary)
    coroutine.yield()
    global_data.level_animation_data = {}
    clean_memory()
    coroutine.yield()
    remove_all_navmesh()
    coroutine.yield()
end

return level
