require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")
require("engine_libs.objects.input")
require("engine_libs.objects.time")
require("engine_libs.objects.global_data")
require("engine_libs.objects.window")
require("engine_libs.short_cuts.create_ui")
require("engine_libs.short_cuts.create_sound")
require("engine_libs.objects.global_data")
require("engine_libs.objects.window")
local serializer = require("libs.serialize")
require("math")



menu_obj = {}

this_object = {}



menu_types = {
    pause = "pause",
    start = "start",
    config = "config",
}

in_main_menu = 0

menu_selectred = "pause"

pause_menu_objects = {}

start_menu_objects = {}

config_menu_objects = {}

in_menu = false

function exit_to_pause_menu()

    menu_selectred = "pause"

    for index, value in pairs(start_menu_objects) do
        value:END()
    end
    start_menu_objects = {}

    for index, value in pairs(config_menu_objects) do
        value:END()
    end
    config_menu_objects = {}

    slider_objects = {}

end

function save_config_and_exit_to_pause_menu()
    save_configs()
    exit_to_pause_menu()
end



function new_game()
    print("new_game")
    core_obj = game_object:new(global_data:get_var("core_object_ptr"))
    core_obj.components[components.lua_scripts]:call_function("core", "load_sceane", {"test_map"})
    --core_obj.components[components.lua_scripts]:call_function("core", "load_sceane", {"test_cut_cene"})
end

function load_game()
    print("load_game")
end

function call_start_menu()
    
    menu_selectred = "start"

    local style = ui_style:new()
    style.border_size = 0
    style.color = { r = 0.2, g = 0.2, b = 0.2, a = 1 }
    style.color_click = { r = 0, g = 0, b = 0, a = 0 }
    start_menu_objects.back_ground_image = create_ui(this_object.object_ptr, { x = -1, y = 0.6, z = 0 }, { x = 2, y = 1.6, z = 2 }, 4,style, "", 0, "resources/Textures/white.png", nil, ui_category.display)
    
    

    style.text_color = { r = 0, g = 1, b = 1, a = 1 }
    style.color_hover = { r = 0, g = 0, b = 0, a = 0 }
    style.border_size = 0.1
    style.border_color = { r = 0, g = 0, b = 0, a = 0 }
    style.border_color_hover = { r = 1, g = 1, b = 1, a = 1 }
    start_menu_objects.new_game_button = create_ui(this_object.object_ptr, { x = -0.5, y = 0.5, z = 0 }, { x = 1, y = 0.25,z = 2 }, 4, style, "NEW GAME", 0.075, "resources/Textures/white.png", new_game, ui_category.button)

    style.text_color = { r = 1, g = 1, b = 0, a = 1 }
    start_menu_objects.load_game_button = create_ui(this_object.object_ptr, { x = -0.6, y = 0, z = 0 }, { x = 1.2, y = 0.25,z = 2 }, 4, style, "LOAD GAME", 0.075, "resources/Textures/white.png", load_game, ui_category.button)

    style.text_color = { r = 1, g = 0, b = 0, a = 1 }
    start_menu_objects.exit_button = create_ui(this_object.object_ptr, { x = -0.2, y = -0.5, z = 0 }, { x = 0.4, y = 0.25, z = 2 }, 4, style, "BACK", 0.075, "resources/Textures/white.png", exit_to_pause_menu, ui_category.button)

    
end




function set_full_screen()
    window:get()
    window.full_screen = not window.full_screen 
    window:set()
end


function increase_sensitivity()
    global_data:set_var("mouse_sensitivity", global_data:get_var("mouse_sensitivity") + 1)
end
function set_sensitivity(sensitivity)
    print("sensitivity",sensitivity)
    local sensitivityValue = tonumber(sensitivity)
    if sensitivityValue then
        global_data:set_var("mouse_sensitivity", sensitivityValue)
    else
        print("insert an valid value")
    end
end
function decrease_sensitivity()
    global_data:set_var("mouse_sensitivity", global_data:get_var("mouse_sensitivity") - 1)
end

function increase_volume()
    get_set_global_volume(get_set_global_volume() + 10)
    
end
function set_volume(volume)
    print("volume",volume)
    local volumeValue = tonumber(volume)
    if volumeValue then
        if volumeValue > 200 then
            volumeValue = 200
        elseif volumeValue < 0 then
            volumeValue = 0
        end
        get_set_global_volume(volumeValue)
    else
        print("insert an valid value")
    end
end
function decrease_volume()
    get_set_global_volume(get_set_global_volume() - 10)
end

function save_configs()
    window:get()
    configs = {
        volume = get_set_global_volume(),
        mouse_sensitivity = global_data:get_var("mouse_sensitivity"),
        full_screen = window.full_screen
    }

    serializer.save_table("config/configs_save.lua",configs)

end

function call_config_menu()
    
    menu_selectred = "config"

    local style = ui_style:new()
    style.border_size = 0
    style.color = { r = 0.25, g = 0.25, b = 0.25, a = 1 }
    style.color_click = { r = 0, g = 0, b = 0, a = 0 }
    config_menu_objects.back_ground_image = create_ui(this_object.object_ptr, { x = -1, y = 1, z = 0 }, { x = 2, y = 2, z = 2 }, 4,style, "", 0, "resources/Textures/white.png", nil, ui_category.display)

    style.color = { r = 0, g = 0, b = 0, a = 0 }
    style.text_color = { r = 1, g = 1, b = 0, a = 1 }
    config_menu_objects.title = create_ui(this_object.object_ptr, { x = -1, y = 1.75, z = 0 }, { x = 2, y = 2, z = 2 },4, style, "CONFIG", 0.2, "resources/Textures/white.png", nil, ui_category.display)

    style.text_color = { r = 1, g = 0, b = 0, a = 1 }
    style.color_hover = { r = 0, g = 0, b = 0, a = 0 }
    style.border_size = 0.1
    style.border_color = { r = 0, g = 0, b = 0, a = 0 }
    style.border_color_hover = { r = 1, g = 1, b = 1, a = 1 }
    config_menu_objects.exit_button = create_ui(this_object.object_ptr, { x = -1, y = 0.9, z = 0 }, { x = 0.2, y = 0.2, z = 2 }, 4, style, "<", 0.075, "resources/Textures/white.png", save_config_and_exit_to_pause_menu, ui_category.button)


    style.text_color = { r = 0, g = 1, b = 0, a = 1 }
    config_menu_objects.volume_controler_button_decrease = create_ui(this_object.object_ptr, { x = -1, y = 0.5, z = 0 }, { x = 0.2, y = 0.25, z = 2 }, 4, style, "<", 0.075, "resources/Textures/null.png", decrease_volume, ui_category.button)
    config_menu_objects.volume_controler = create_ui(this_object.object_ptr, { x = -0.8, y = 0.5, z = 0 }, { x = 1.6, y = 0.25,z = 2 }, 4, style, "volume: " .. get_set_global_volume(), 0.075, "resources/Textures/null.png", set_volume, ui_category.input_fild)
    config_menu_objects.volume_controler_button_increase = create_ui(this_object.object_ptr, { x = 0.8, y = 0.5, z = 0 }, { x = 0.2, y = 0.25, z = 2 }, 4, style, ">", 0.075, "resources/Textures/null.png", increase_volume, ui_category.button)

    config_menu_objects.sensitivity_controler_button_decrease = create_ui(this_object.object_ptr, { x = -1, y = 0, z = 0 }, { x = 0.2, y = 0.25, z = 2 }, 4, style, "<", 0.075, "resources/Textures/null.png", decrease_sensitivity, ui_category.button)
    config_menu_objects.mouse_sensitivity = create_ui(this_object.object_ptr, { x = -0.8, y = 0, z = 0 }, { x = 1.6, y = 0.25,z = 2 }, 4, style, "mouse_sensitivity: " .. global_data:get_var("mouse_sensitivity"), 0.05, "resources/Textures/null.png", set_sensitivity, ui_category.input_fild)
    config_menu_objects.sensitivity_controler_button_increase = create_ui(this_object.object_ptr, { x = 0.8, y = 0, z = 0 }, { x = 0.2, y = 0.25, z = 2 }, 4, style, ">", 0.075, "resources/Textures/null.png", increase_sensitivity, ui_category.button)
    local is_full_screen = "false"
    
    
    
    if window.full_screen then
        is_full_screen = "true"
    end
    config_menu_objects.full_screen_controler  = create_ui(this_object.object_ptr, { x = -0.8, y = -0.5, z = 0 }, { x = 1.6, y = 0.25,z = 2 }, 4, style, "full_screen: " .. is_full_screen, 0.05, "resources/Textures/null.png", set_full_screen, ui_category.button)
    

end






function exit()
    window:close()
end


function START()

    global_data:set_var("pause", 1)
    time:set_speed(0)

    local layers = global_data:get_var("layers")

    this_object = game_object:new(this_object_ptr)
    
    local style = ui_style:new()

    style.border_size = 0
    style.color = { r = 0, g = 0.2, b = 0.2, a = 1 }
    style.color_click = { r = 0, g = 0, b = 0, a = 0 }

    if in_main_menu ~= 0 then
        pause_menu_objects.back_ground_image = create_ui(this_object.object_ptr, { x = -1, y = 1, z = 0 }, { x = 2, y = 2, z = 2 }, 4,style, "", 0, "resources/Textures/null.png", nil, ui_category.display)
    end

    style.color = { r = 0, g = 0, b = 0, a = 0 }
    pause_menu_objects.title = create_ui(this_object.object_ptr, { x = -1, y = 1.75, z = 0 }, { x = 2, y = 2, z = 2 },4, style, "HMTF", 0.2, "resources/Textures/null.png", nil, ui_category.display)

    style.color_hover = { r = 0, g = 0, b = 0, a = 0 }
    style.text_color = { r = 0.25, g = 1, b = 1, a = 1 }
    style.border_size = 0.1
    style.border_color = { r = 0, g = 0, b = 0, a = 0 }
    style.border_color_hover = { r = 1, g = 1, b = 1, a = 1 }
    if in_main_menu ~= 0 then
        pause_menu_objects.start_button = create_ui(this_object.object_ptr, { x = -0.25, y = 0.5, z = 0 }, { x = 0.5, y = 0.25,z = 2 }, 4, style, "START", 0.075, "resources/Textures/null.png", call_start_menu, ui_category.button)
    end
    style.text_color = { r = 1, g = 1, b = 0, a = 1 }
    pause_menu_objects.config_button = create_ui(this_object.object_ptr, { x = -0.3, y = 0, z = 0 }, { x = 0.6, y = 0.25, z = 2 },4, style, "CONFIG", 0.075, "resources/Textures/null.png", call_config_menu, ui_category.button)

    style.text_color = { r = 1, g = 0, b = 0, a = 1 }
    pause_menu_objects.exit_button = create_ui(this_object.object_ptr, { x = -0.2, y = -0.5, z = 0 }, { x = 0.4, y = 0.25, z = 2 }, 4, style, "QUIT", 0.075, "resources/Textures/null.png", exit, ui_category.button)


end





function UPDATE()

    

    if menu_selectred == "pause" then
        for key, value in pairs(pause_menu_objects) do
            if value.UPDATE ~= nil then
                value:UPDATE()
            end
        end
    end
    
    if menu_selectred == "start" then

        if tablelength(start_menu_objects) == 0 then
            call_start_menu()
        end

        for key, value in pairs(start_menu_objects) do
            if value.UPDATE ~= nil then
                value:UPDATE()
            end
        end
    end

    if menu_selectred == "config" then
        if tablelength(config_menu_objects) == 0 then
            call_config_menu()
        end

        config_menu_objects.volume_controler.text = "volume: " .. get_set_global_volume()

        config_menu_objects.mouse_sensitivity.text = "mouse_sensitivity: " .. global_data:get_var("mouse_sensitivity")

        window:get()
        local is_full_screen = "false"
        if window.full_screen then
            is_full_screen = "true"
        end
        config_menu_objects.full_screen_controler.text  =  "full_screen: " .. is_full_screen 

        for key, value in pairs(config_menu_objects) do
            if value.UPDATE ~= nil then
                value:UPDATE()
            end
        end

        
    end

    
    

    
    
end

function END()
    time:set_speed(1)
    global_data:set_var("pause", 0)
end

function COLLIDE(collision_info)
end
