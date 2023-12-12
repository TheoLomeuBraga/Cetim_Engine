require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.input")
require("objects.time")
require("objects.global_data")
require("objects.window")
require("short_cuts.create_ui")
require("short_cuts.create_sound")
require("objects.window")
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

    show_pause_menu(true)
end

function save_config_and_exit_to_pause_menu()
    save_configs()
    show_pause_menu(true)
end

function new_game()
    print("new_game")
    core_obj = game_object(global_data:get_var("core_object_ptr"))
    --core_obj.components.lua_scripts:call_function("core", "load_sceane", {"test_map"})
    core_obj.components.lua_scripts:call_function("core", "load_sceane", { "hub_map" })
end

function load_game()
    print("load_game")
end

function call_start_menu(state, id)
    if state == "click" then
        menu_selectred = "start"

        show_pause_menu(false)


        local style = ui_style:new()
        style.border_size = 0
        style.color = { r = 0.2, g = 0.2, b = 0.2, a = 1 }
        style.color_click = { r = 0, g = 0, b = 0, a = 0 }
        start_menu_objects.back_ground_image = create_ui(this_object.object_ptr, { x = -1, y = 0.6, z = 0 },
            { x = 2, y = 1.6, z = 2 }, 5, style, "", 0, "resources/Textures/white.png", nil, ui_category.display)



        style.text_color = { r = 0, g = 1, b = 1, a = 1 }
        style.color_hover = { r = 0, g = 0, b = 0, a = 0 }
        style.border_size = 0.1
        style.border_color = { r = 0, g = 0, b = 0, a = 0 }
        style.border_color_hover = { r = 1, g = 1, b = 1, a = 1 }
        start_menu_objects.new_game_button = create_ui(this_object.object_ptr, { x = -0.5, y = 0.5, z = 0 },
            { x = 1, y = 0.25, z = 2 }, 5, style, "NEW GAME", 0.075, "resources/Textures/white.png", new_game,
            ui_category.button)

        style.text_color = { r = 1, g = 1, b = 0, a = 1 }
        start_menu_objects.load_game_button = create_ui(this_object.object_ptr, { x = -0.6, y = 0, z = 0 },
            { x = 1.2, y = 0.25, z = 2 }, 5, style, "LOAD GAME", 0.075, "resources/Textures/white.png", load_game,
            ui_category.button)

        style.text_color = { r = 1, g = 0, b = 0, a = 1 }
        start_menu_objects.exit_button = create_ui(this_object.object_ptr, { x = -0.2, y = -0.5, z = 0 },
            { x = 0.4, y = 0.25, z = 2 }, 5, style, "BACK", 0.075, "resources/Textures/white.png", exit_to_pause_menu,
            ui_category.button)
    end
end

function set_full_screen()
    window:get()
    window.full_screen = not window.full_screen
    window.resolution.x = 256
    window.resolution.y = 224
    window:set()
end

function increase_sensitivity()
    global_data:set_var("mouse_sensitivity", global_data:get_var("mouse_sensitivity") + 1)
end

function set_sensitivity(sensitivity)
    print("sensitivity", sensitivity)
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
    print("volume", volume)
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

    serializer.save_table("config/configs_save.lua", configs)
end

function call_config_menu(state, id)
    if state == "click" then
        show_pause_menu(false)
        menu_selectred = "config"

        local adv_ui = advanced_ui_style()
        adv_ui.text_color = { r = 1, g = 1, b = 1, a = 1 }
        adv_ui.background_color = { r = 0, g = 0, b = 0, a = 1 }
        adv_ui.border_color = { r = 0, g = 0, b = 0, a = 1 }
        adv_ui.border_size = 0
        adv_ui.background_image = "resources/Textures/null.png"
        adv_ui.border_image = "resources/Textures/null.png"

        --[[
        local style = ui_style:new()
        style.border_size = 0
        style.color = { r = 0.25, g = 0.25, b = 0.25, a = 1 }
        style.color_click = { r = 0, g = 0, b = 0, a = 0 }
        config_menu_objects.back_ground_image = create_ui(this_object.object_ptr, { x = -1, y = 1, z = 0 },{ x = 2, y = 2, z = 2 }, 5, style, "", 0, "resources/Textures/white.png", nil, ui_category.display)
        ]]

        local background_style = advanced_ui_style()
        background_style.text_color = { r = 0.1, g = 0.1, b = 0.1, a = 1 }
        background_style.background_color = { r = 0.1, g = 0.1, b = 0.1, a = 1 }
        background_style.border_color = { r = 0.1, g = 0.1, b = 0.1, a = 1 }
        config_menu_objects.back_ground_image = create_ui_element(this_object.object_ptr,0,{x=0.5,y=0.5},{x=1,y=1},"",nil,background_style)

        --

        style.color = { r = 0, g = 0, b = 0, a = 0 }
        style.text_color = { r = 1, g = 1, b = 0, a = 1 }
        config_menu_objects.title = create_ui(this_object.object_ptr, { x = -1, y = 1.75, z = 0 },
            { x = 2, y = 2, z = 2 }, 5,
            style, "CONFIG", 0.2, "resources/Textures/white.png", nil, ui_category.display)

        style.text_color = { r = 1, g = 0, b = 0, a = 1 }
        style.color_hover = { r = 0, g = 0, b = 0, a = 0 }
        style.border_size = 0.1
        style.border_color = { r = 0, g = 0, b = 0, a = 0 }
        style.border_color_hover = { r = 1, g = 1, b = 1, a = 1 }
        config_menu_objects.exit_button = create_ui(this_object.object_ptr, { x = -1, y = 0.9, z = 0 },
            { x = 0.2, y = 0.2, z = 2 }, 5, style, "<", 0.075, "resources/Textures/white.png",
            save_config_and_exit_to_pause_menu, ui_category.button)


        style.text_color = { r = 0, g = 1, b = 0, a = 1 }
        config_menu_objects.volume_controler_button_decrease = create_ui(this_object.object_ptr,
            { x = -1, y = 0.5, z = 0 },
            { x = 0.2, y = 0.25, z = 2 }, 5, style, "<", 0.075, "resources/Textures/null.png", decrease_volume,
            ui_category.button)
        config_menu_objects.volume_controler = create_ui(this_object.object_ptr, { x = -0.8, y = 0.5, z = 0 },
            { x = 1.6, y = 0.25, z = 2 }, 5, style, "volume: " .. get_set_global_volume(), 0.075,
            "resources/Textures/null.png", set_volume, ui_category.input_fild)
        config_menu_objects.volume_controler_button_increase = create_ui(this_object.object_ptr,
            { x = 0.8, y = 0.5, z = 0 },
            { x = 0.2, y = 0.25, z = 2 }, 5, style, ">", 0.075, "resources/Textures/null.png", increase_volume,
            ui_category.button)
        config_menu_objects.sensitivity_controler_button_decrease = create_ui(this_object.object_ptr,
            { x = -1, y = 0, z = 0 }, { x = 0.2, y = 0.25, z = 2 }, 5, style, "<", 0.075, "resources/Textures/null.png",
            decrease_sensitivity, ui_category.button)
        config_menu_objects.mouse_sensitivity = create_ui(this_object.object_ptr, { x = -0.8, y = 0, z = 0 },
            { x = 1.6, y = 0.25, z = 2 }, 5, style, "mouse_sensitivity: " .. global_data:get_var("mouse_sensitivity"),
            0.05,
            "resources/Textures/null.png", set_sensitivity, ui_category.input_fild)
        config_menu_objects.sensitivity_controler_button_increase = create_ui(this_object.object_ptr,
            { x = 0.8, y = 0, z = 0 }, { x = 0.2, y = 0.25, z = 2 }, 5, style, ">", 0.075, "resources/Textures/null.png",
            increase_sensitivity, ui_category.button)
        local is_full_screen = "false"


        if window.full_screen then
            is_full_screen = "true"
        end
        config_menu_objects.full_screen_controler = create_ui(this_object.object_ptr, { x = -0.8, y = -0.5, z = 0 },
            { x = 1.6, y = 0.25, z = 2 }, 5, style, "full_screen: " .. is_full_screen, 0.05,
            "resources/Textures/null.png",
            set_full_screen, ui_category.button)
    end
end

local block_exit_button = false
function exit(state, id)
    if state == "click" then
        window:close()
    end
end

function show_pause_menu(on)
    if on then
        local adv_ui = advanced_ui_style()

        if in_main_menu ~= 0 then
            adv_ui.text_color = { r = 0, g = 1, b = 0.2, a = 1 }
            adv_ui.background_color = { r = 0, g = 0.2, b = 0.2, a = 1 }
            adv_ui.border_color = { r = 0, g = 0.2, b = 0.2, a = 1 }
            adv_ui.border_size = 0
            adv_ui.background_image = "resources/Textures/null.png"
            adv_ui.border_image = "resources/Textures/null.png"

            pause_menu_objects.back_ground_image = create_ui_element(this_object.object_ptr, ui_types.common,
                { x = 0.5, y = 0.5 }, { x = 1, y = 1 }, "", nil, adv_ui)
        end

        adv_ui.text_color = { r = 0, g = 1, b = 0, a = 0 }
        adv_ui.background_color = { r = 0, g = 0, b = 0, a = 0 }
        adv_ui.border_color = { r = 0, g = 0, b = 0, a = 0 }

        adv_ui.text_size = 0.2

        pause_menu_objects.base_object = create_ui_element(this_object.object_ptr, ui_types.common,
            { x = 0, y = 0 },
            { x = 0.0, y = 0.0 }, "", "call_start_menu", { normal_style, hover_style })

        pause_menu_objects.title = create_ui_element(pause_menu_objects.base_object.object_ptr, ui_types.common,
            { x = 0.5, y = 0.85 }, { x = 2, y = 2 }, "HMTF", nil, adv_ui)

        --button start

        local transparent_color = { r = 0, g = 0, b = 0, a = 0 }
        local hover_border_color = { r = 1, g = 1, b = 1, a = 1 }

        local normal_style = advanced_ui_style()
        normal_style.background_color = transparent_color
        normal_style.border_size = 0
        normal_style.text_color = { r = 0.25, g = 1, b = 1, a = 1 }
        normal_style.border_color = transparent_color
        normal_style.background_color = transparent_color
        normal_style.background_image = "resources/Textures/white.png"
        normal_style.border_image = "resources/Textures/white.png"
        normal_style.text_size = 0.075

        local hover_style = deepcopy(normal_style)
        hover_style.border_color = hover_border_color
        hover_style.background_color = hover_border_color



        pause_menu_objects.start_button = create_ui_element(pause_menu_objects.base_object.object_ptr,
            ui_types.common, { x = 0.5, y = 0.7 }, { x = 0.5, y = 0.2 }, "START", "call_start_menu",
            { normal_style, hover_style })

        normal_style.text_color = { r = 1, g = 1, b = 0, a = 1 }
        pause_menu_objects.config_button = create_ui_element(pause_menu_objects.base_object.object_ptr,
            ui_types.common, { x = 0.5, y = 0.45 }, { x = 0.5, y = 0.2 }, "CONFIG", "call_config_menu",
            { normal_style, hover_style })

        normal_style.text_color = { r = 1, g = 0, b = 0, a = 1 }
        pause_menu_objects.config_button = create_ui_element(pause_menu_objects.base_object.object_ptr,
            ui_types.common, { x = 0.5, y = 0.2 }, { x = 0.5, y = 0.2 }, "QUIT", "exit", { normal_style, hover_style })
    else
        remove_object(pause_menu_objects.base_object.object_ptr)
    end
end

function START()
    global_data:set_var("pause", 1)
    time:set_speed(0)

    local layers = global_data:get_var("layers")

    this_object = game_object(this_object_ptr)

    show_pause_menu(true)
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
        config_menu_objects.full_screen_controler.text = "full_screen: " .. is_full_screen

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
