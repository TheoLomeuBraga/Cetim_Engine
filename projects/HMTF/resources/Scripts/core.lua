require("TMP_libs.definitions")
require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.game_object")
require("TMP_libs.objects.time")

require("TMP_libs.objects.layers_table")
require("TMP_libs.objects.render_layer")

require("math")
json = require("libs.json")

require("TMP_libs.objects.material")
require("TMP_libs.objects.window")

require("TMP_libs.objects.global_data")
require("TMP_libs.short_cuts.create_camera")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.objects.layers_table")
require("TMP_libs.objects.vectors")
require("TMP_libs.objects.input")
require("TMP_libs.objects.gravity")

require("TMP_libs.objects.render_layer")

cam = {}
layers = layers_table:new_3D()

back_ground = {}

demo_selected = 1
demos_list = { "main_menu", "3D", "text", "2D", "buttons_test" }
demo = nil
function load_demo(demo_name)
    if demo ~= nil then
        demo:END()
    end
    demo = nil
    demo = require("level_loaders." .. demo_name)
    print(demo)
    demo:START(layers)
end

function set_render_layers()
    renders_layers.layers = { render_layer:new(), render_layer:new(), render_layer:new() }

    renders_layers.layers[1] = {
        camera_selected = 0,
        start_render = true,
        clean_color = true,
        clean_deep = true,
        enable = true,
        end_render = true,
        use_deep = true,
    }

    renders_layers.layers[2] = {
        camera_selected = 0,
        start_render = false,
        clean_color = false,
        clean_deep = true,
        enable = true,
        end_render = true,
        use_deep = true,
    }

    renders_layers.layers[3] = {
        camera_selected = 0,
        start_render = false,
        clean_color = false,
        clean_deep = true,
        enable = true,
        end_render = true,
        use_deep = true,
    }

    renders_layers.layers[4] = {
        camera_selected = 0,
        start_render = false,
        clean_color = false,
        clean_deep = true,
        enable = true,
        end_render = true,
        use_deep = false,
    }

    renders_layers:set()
end

function START()
    set_render_layers()
    --get_set_parallel_loading(set_lua, true)
    local mat = matreial:new()
    mat.shader = "resources/Shaders/background"
    mat.textures[1] = "resources/Textures/white.png"
    mat.color.r = 0.2
    mat.color.g = 0.2
    mat.color.b = 0.2
    back_ground = create_render_shader(create_object(), true, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(1, 1, 1), 1, mat)

    window.resolution.x = 720
    window.resolution.y = 720
    window:set()

    gravity:set()

    global_data:set_var("core_object_ptr", this_object_ptr)

    layers:create()

    cam = create_camera_perspective(layers.camera, { x = 0, y = 0, z = -10 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    cam:add_component(components.lua_scripts)
    cam.components[components.lua_scripts]:add_script("game_scripts/free_camera")
    set_lisener_object(cam.object_ptr)
    set_global_volume(100)

    load_demo(demos_list[demo_selected])
end

function next_demo()
    demo_selected = demo_selected + 1
    if demo_selected > tablelength(demos_list) then
        demo_selected = 1
    end
    load_demo(demos_list[demo_selected])
end

function previous_demo()
    demo_selected = demo_selected - 1
    if demo_selected < 1 then
        demo_selected = tablelength(demos_list)
    end
    load_demo(demos_list[demo_selected])
end

keys_pressed = { q = false, e = false }
keys_pressed_last_frame = { q = false, e = false }

function UPDATE()
    demo:UPDATE()

    keys_pressed.q = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.q])
    keys_pressed.e = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.e])

    if keys_pressed.q == 1 and keys_pressed_last_frame.q == 0 then
        previous_demo()
    end

    if keys_pressed.e == 1 and keys_pressed_last_frame.e == 0 then
        next_demo()
    end

    keys_pressed_last_frame.q = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.q])
    keys_pressed_last_frame.e = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.e])

    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.delete]) == 1 then
        window:close()
    end
end

function COLLIDE(collision_info)
end

function END()
    window:close()
end
