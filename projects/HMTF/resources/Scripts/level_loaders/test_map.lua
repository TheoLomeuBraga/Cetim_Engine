require("engine_libs.short_cuts.create_mesh")
require("engine_libs.short_cuts.create_collision")
require("engine_libs.short_cuts.create_sound")
require("engine_libs.objects.time")
require("engine_libs.objects.scene_3D")
require("resources.playable_scene")

local level = {
    cenary_obj = {},
    scene_3D_data = {},
}

core_obj = {}

local assets_needed = {
    new_asset_list_element(asset_types.scene_3D,"resources/Levels/3D/test_map.gltf")
}

function level:START(layers)
    
    time:set_speed(0)

    core_obj = game_object:new(global_data:get_var("core_object_ptr"))
    core_obj.components[components.lua_scripts]:call_function("core","set_load_image",{path = "resources/Textures/null.png",color={r=1,g=1,b=1}})

    local camera = create_camera_perspective(layers.camera, { x = 0, y = 0, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    set_lisener_object(camera.object_ptr)

    stop_coroutine_if_is_not_loaded(assets_needed)

    level.scene_3D_data = get_scene_3D("resources/Levels/3D/test_map.gltf")

    coroutine.yield()
    print("yield 1")

    cenary_builders.cenary_obj = cenary_builders.scene(layers.hud,level.scene_3D_data,true)

    coroutine.yield()
    print("yield 2")

    core_obj.components[components.lua_scripts]:call_function("core","set_load_image",{})

    coroutine.yield()
    print("yield 3")

    time:set_speed(1)
    remove_object(camera.object_ptr)
    camera = {}
    
end





function level:UPDATE()

    
    

end

function level:END()
    clear_memory()
end

return level
