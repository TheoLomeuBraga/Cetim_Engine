require("definitions")
require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.game_object")
require("objects.time")

require("objects.layers_table")
require("objects.render_layer")

require("math")
json = require("libs.json")

require("objects.material")
require("objects.window")

require("objects.global_data")
require("short_cuts.create_camera")
require("short_cuts.create_render_shader")
require("short_cuts.create_mesh")
require("objects.layers_table")
require("objects.vectors")
require("objects.input")
require("objects.gravity")

require("objects.render_layer")
require("objects.post_processing")

require("function_sets.simple_ui")


local serializer = require("libs.serialize")

register_function_set("file_system")

local config_path = get_config_folder_path() .. "/hmtf"
local config_file_path = config_path .. "/configs_save.lua"

local this_object = {}



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
    mat.shader = "background"
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
        
        previous_cenary = cenary

        cenary = require("Levels." .. name .. ".loader")
        
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

load_sceane_request_name = nil

function load_sceane(cenary_name)
    
    
    
    sceane_name = cenary_name
    loader = coroutine.create(load_sceane_step)
    unloader = coroutine.create(unload_sceane_step)
    load_sceane_request_name = nil
    
end

function load_sceane_request(cenary_name)
    
    load_sceane_request_name = cenary_name
    
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

    --[[
   local t = {"ola mundo",{1,2,3}}
   print(t[1],t[2][1],t[2][2],t[2][3])
   serializer.save_table_compressed("a.bin",t)
   ]]
   
   
   --[[
   t = serializer.load_table_compressed("a.bin")
   print(t[1],t[2][1],t[2][2],t[2][3])
   ]]
   

    configs = serializer.load_table(config_file_path)
    if configs ~= nil then
        serializer.save_table(config_file_path,configs)
        get_set_global_volume(configs.volume)
        global_data.mouse_sensitivity=configs.mouse_sensitivity
        window.full_screen = configs.full_screen
        window:set()
    else
        get_set_global_volume(100)
        global_data.mouse_sensitivity = 6
        window.full_screen = false
        window:set()
    end
end



function START()


    post_processing:get()
    post_processing.material = matreial:new()
    post_processing.material.shader = "post_processing"
    post_processing:set()
    
    layers:create()

    set_render_layers()

    
    local mat = matreial:new()
    mat.shader = "background"
    mat.textures[1] = "Textures/white.png"
    mat.color.r = 0.2
    mat.color.g = 0.2
    mat.color.b = 0.2
    back_ground = create_render_shader(layers.sky_box, true, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(1, 1, 1), 1, mat)


    window.resolution.x = 256
    window.resolution.y = 224

    window:set()
    gravity:set()
    

    global_data.core_object_ptr = this_object_ptr
    global_data.mouse_sensitivity = 6
    global_data.layers = layers

    this_object = game_object(this_object_ptr)
    this_object.components.lua_scripts:add_script("game_scripts/input_geter")
    this_object.components.lua_scripts:add_script("game_scripts/bullet_master")
    this_object.components.lua_scripts:add_script("game_scripts/entity_master")
    
    
    
    
    load_configs()
    load_sceane("main_menu")
    global_data.localization_file = "localization/dialog.json"

    
    
end

local count_fps ={
    time = 0,
    frames = 0,
}
function count_fps:update()
    time:get()
    self.time = self.time + time.delta
    self.frames = self.frames + 1
    if self.time >= 1 then
        self.time = 0
        print("FPS",self.frames)
        self.frames = 0
    end
end

local ui_next_pos = {x=0,y=0}
cursor_pos_last_frame = {x=0,y=0,z=0}

local reload_last_frame = false
function bool_to_num(num)
    if num then
        return 1
    end
    return 0
end
function UPDATE()

    if global_data.inputs ~= nil then
        local inputs = global_data.inputs
        local inputs_last_frame = global_data.inputs_last_frame

        local cursor_movement = {x=inputs.mouse_pos_x-cursor_pos_last_frame.x,y=cursor_pos_last_frame.y-inputs.mouse_pos_y}
        set_selection_state({x=inputs.mouse_pos_x,y=inputs.mouse_pos_y},cursor_movement,-1,inputs.action_1)
        cursor_pos_last_frame = {x=inputs.mouse_pos_x,y=inputs.mouse_pos_y}
    end 
    

    local ui_transform = simple_ui_transform()
    ui_transform.position = {x=ui_next_pos.x,y=0.30 + ui_next_pos.y,z=0}
    ui_transform.scale = {x=0.25,y=0.05}
    local slider_result = simple_ui_slider(ui_transform,"window",nil,1)
    ui_next_pos.x = ui_next_pos.x + (slider_result.x * 2)
    ui_next_pos.y = ui_next_pos.y + (slider_result.y * 2)
    
    
    ui_transform = simple_ui_transform()
    ui_transform.position = {x=ui_next_pos.x,y=ui_next_pos.y,z=0}
    ui_transform.scale = {x=0.25,y=0.25}
   
    if simple_ui_button(ui_transform,"push me",nil,1) == 1 then
        window:close()
    end

    count_fps:update()
    
    if not keep_loading() and cenary ~= nil then
        cenary:UPDATE()
    end
    keep_unloading()

    local last_frame = keys_axis:get_input(input_devices.keyboard, "r") == 1

    if not reload_last_frame and last_frame then
        load_sceane(sceane_name)
    end
    reload_last_frame = last_frame

    if load_sceane_request_name ~= nil then
        load_sceane(load_sceane_request_name)
    end

    if keys_axis:get_input(input_devices.keyboard, "delete") == 1 then
        window:close()
    end
end

function COLLIDE(collision_info)
end

function END()
    window:close()
end
