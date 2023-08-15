require("engine_libs.definitions")
require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")
require("engine_libs.objects.game_object")
require("engine_libs.objects.time")

require("engine_libs.objects.layers_table")
require("engine_libs.objects.render_layer")

require("math")
json = require("libs.json")

require("engine_libs.objects.material")
require("engine_libs.objects.window")

require("engine_libs.objects.global_data")
require("engine_libs.short_cuts.create_camera")
require("engine_libs.short_cuts.create_render_shader")
require("engine_libs.objects.layers_table")
require("engine_libs.objects.vectors")
require("engine_libs.objects.input")
require("engine_libs.objects.gravity")

require("engine_libs.objects.render_layer")

local serializer = require("libs.serialize")

local this_object



cam = {}
layers = layers_table:new_3D()

back_ground = {}


sceane_name = ""
local loader = coroutine.create(function () end)
function load_sceane_step()
    name = ""
    
    if type(sceane_name) == "string" then
        name = sceane_name
    elseif type(sceane_name) == "table" then
        for key, value in pairs(sceane_name) do
            name = value
        end
    end
    
    if type(sceane_name) == "string" or type(sceane_name) == "table" then
        if demo ~= nil then
            demo:END()
            
        end

        demo = require("level_loaders." .. name)
        
        demo:START(layers)
    end
    
    
end
function load_sceane(demo_name)
    sceane_name = demo_name

    --load_sceane_step()
    loader = coroutine.create(load_sceane_step)
end

function keep_loading()
    if coroutine.status(loader) ~= "dead" then
        coroutine.resume(loader)
        return true
    end
    return false
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
        window.full_screen = configs.full_screen
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

    this_object = game_object:new(create_object(this_object_ptr))
    this_object:add_component(components.lua_scripts)
    this_object.components[components.lua_scripts]:add_script("game_scripts/input_getter")

    

    load_configs()

    load_sceane("main_menu")
    
end

function UPDATE()
    if keep_loading() and demo ~= nil then
        demo:UPDATE()
    end
    --

    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.delete]) == 1 then
        window:close()
    end
end

function COLLIDE(collision_info)
end

function END()
    window:close()
end
