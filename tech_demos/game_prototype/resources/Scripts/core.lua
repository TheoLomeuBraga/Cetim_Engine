require("TMP_libs.definitions")
require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.game_object")
require("TMP_libs.objects.time")
require("TMP_libs.stystems")
require("TMP_libs.layers_table")
require("TMP_libs.objects.render_layer")




require("math")
json = require("libs.json")


require("TMP_libs.short_cuts.load_2D_map")
require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.fps_counter")
require("TMP_libs.short_cuts.create_render_shader")

require("TMP_libs.material")
require("TMP_libs.objects.window")


require("run_comunication_test")

require("TMP_libs.objects.input")


require("TMP_libs.objects.scene_3D")

require("TMP_libs.objects.global_data")

function set_resolution()
    window.resolution.x = 720
    window.resolution.y = 720
    window:set()
end

current_scene = {}
function load_map(file)
    current_scene = nil
    current_scene = require("game_maps." .. file)
    current_scene:load()
end

function unload_map()
    current_scene:unload()
end

function test_load_unload(file)
    local i = 0
    while i < 2 do
        print("load_map")
        load_map(file)
        print("unload_map")
        unload_map()
        i = i + 1
    end
    load_map(file)
end

function START()
    print("core iniciando")
    full_test()
    set_resolution()
    
    global_data:set_var("core_object_ptr",this_object_ptr)

    test_get_scene_3D = get_scene_3D("resources/3D Models/cube.gltf")


    --load_map("test_2D_map")
    --load_map("test_3D_physics_map")
    --load_map("hello_world")
    --load_map("test_3D_assets")
    load_map("test_3D_game")

    
    
end

function UPDATE()
    if current_scene.update ~= nil then
        current_scene:update()
    end
    --[[
    keys_axis:set_text_input_geter(true)
    print(keys_axis:get_text_input())
    ]]
    --[[
    if keys_axis:get_input(input_devices.keyboard,"a") == 1 then
        print("a",keys_axis:get_input(input_devices.keyboard,"a"))
    end
    ]]
    --[[
    l = keys_axis:get_input(input_devices.mouse, "left")
    r = keys_axis:get_input(input_devices.mouse, "right")
    print("mouse movement", l, r)
    ]]
    --[[
    if keys_axis:get_input_joystick(1,"a") == 1 then
        print("a",keys_axis:get_input_joystick(1,"a"))
    end
    ]]
    --[[
    rx = keys_axis:get_input_joystick(1,"rx")
    if rx > 0 then
        print("AAAAA:",rx)
    end
    ]]
end

function COLLIDE(collision_info)
end

function END()
end
