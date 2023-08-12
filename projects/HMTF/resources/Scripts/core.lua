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

local serializer = require("libs.serialize")

cam = {}
layers = layers_table:new_3D()

back_ground = {}




function load_sceane(demo_name)
    name = ""

    if type(demo_name) == "string" then
        name = demo_name
    elseif type(demo_name) == "table" then
        for key, value in pairs(demo_name) do
            name = value
        end
    end

    if demo ~= nil then
        demo:END()
        demo = nil
    end
    demo = require("level_loaders." .. name)
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

function load_configs()
    configs = serializer.load_table("config/configs_save.lua")
    if configs ~= nil then
        serializer.save_table("config/configs_save.lua",configs)
        get_set_global_volume(configs.volume)
        global_data:set_var("mouse_sensitivity",configs.mouse_sensitivity)
        window.full_screen = configs.full_screen == 1
        window:set()
    else
        get_set_global_volume(100)
        global_data:set_var("mouse_sensitivity",6)
        window.full_screen = false
        window:set()
    end
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

    

    layers:create()

    global_data:set_var("core_object_ptr", this_object_ptr)
    global_data:set_var("mouse_sensitivity", 6)
    global_data:set_var("layers", layers)
    
    

    load_sceane("main_menu")

    load_configs()
    
end

function UPDATE()
    demo:UPDATE()

    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.delete]) == 1 then
        window:close()
    end
end

function COLLIDE(collision_info)
end

function END()
    window:close()
end
