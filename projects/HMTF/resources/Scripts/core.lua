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

load_image = nil

cenary = nil
previous_cenary = nil

function set_load_image(args)

    if load_image ~= nil then
        remove_object(load_image.object_ptr)
        load_image = nil
    end

    local mat = matreial:new()
    mat.shader = "resources/Shaders/background"
    if args.path ~= nil and args.path ~= "" then
        mat.textures[1] = args.path
        if args.color ~= nil then
            mat.color.r = args.color.r
            mat.color.g = args.color.g
            mat.color.b = args.color.b 
        end
        mat.color.a = 1

        load_image = create_render_shader(layers.hud, true, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(1, 1, 1), 5, mat)
    else
        remove_object(load_image.object_ptr)
        load_image = nil
    end
    
end


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
        
        previous_cenary = deepcopy(cenary)

        cenary = require("level_loaders." .. name)
        cenary:START()
        
    end
    
    
    
end

local unloader = coroutine.create(function () end)
function unload_sceane_step()
    
    if previous_cenary ~= nil then
        previous_cenary:END()
        previous_cenary = nil
    end
    
end

function load_sceane(cenary_name)
    sceane_name = cenary_name
    loader = coroutine.create(load_sceane_step)
    unloader = coroutine.create(unload_sceane_step)
end

function keep_loading()
    if coroutine.status(loader) ~= "dead" then
        coroutine.resume(loader)
        return true
    end
    return false
end

function keep_unloading()
    if coroutine.status(unloader) ~= "dead" then
        coroutine.resume(unloader)
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
        use_deep = true,
    }

    renders_layers.layers[5] = {
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
    
    layers:create()

    set_render_layers()
    --get_set_parallel_loading(set_lua, true)
    local mat = matreial:new()
    mat.shader = "resources/Shaders/background"
    mat.textures[1] = "resources/Textures/white.png"
    mat.color.r = 0.2
    mat.color.g = 0.2
    mat.color.b = 0.2
    back_ground = create_render_shader(layers.sky_box, true, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(1, 1, 1), 1, mat)

    --[[
    mat.shader = "resources/Shaders/tiled_volume"
    mat.textures[1] = "resources/Textures/null.png"
    mat.color.r = 1
    mat.color.g = 1
    mat.color.b = 1
    create_render_shader(create_object(), false, Vec3:new(0, 2, 5), Vec3:new(0, 0, 0), Vec3:new(1, 1, 1), 2, mat)
    ]]

    --mat.color.a = 0
    --create_render_shader(create_object(), true, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(1, 1, 1), 4, mat)

    window.resolution.x = 720
    window.resolution.y = 720

    window:set()
    gravity:set()
    

    global_data:set_var("core_object_ptr", this_object_ptr)
    global_data:set_var("mouse_sensitivity", 6)
    global_data:set_var("layers", layers)

    this_object = game_object:new(create_object(this_object_ptr))
    this_object:add_component(components.lua_scripts)
    this_object.components[components.lua_scripts]:add_script("game_scripts/input_geter")

    

    load_configs()

    load_sceane("main_menu")

    global_data:set_var("localization_file","resources/localization/AmericanEnglish.json")
    
end

function UPDATE()

    

    if keep_loading() and cenary ~= nil then
        cenary:UPDATE()
    end
    keep_unloading()

    if keys_axis:get_input(input_devices.keyboard, "delete") == 1 then
        window:close()
    end
end

function COLLIDE(collision_info)
end

function END()
    window:close()
end
