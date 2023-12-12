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

local empty_style = advanced_ui_style()
empty_style.text_color = {r=0,g=0,b=0,a=0}
empty_style.background_color = {r=0,g=0,b=0,a=0}
empty_style.border_color = {r=0,g=0,b=0,a=0}
empty_style.border_size = 0
empty_style.text_size = 0

menu_types = {
    main_menu = "main_menu",
    pause = "pause",
    start = "start",
    config = "config",
}
menu_selectred = "pause"

local menu_objects = {}
local menus_locations = {}

function start_main_menu()
    --background
    local background_style = deepcopy(empty_style)
    background_style.background_color = { r = 0, g = 0.2, b = 0.2, a = 1 }
    background_style.text_color = {r=0,g=1,b=0,a=1}
    background_style.background_image = "resources/Textures/null.png"
    menu_objects.back_ground_image = create_ui_element(menu_objects.base.object_ptr, ui_types.common,{ x = 0.5, y = 0.5 }, { x = 1, y = 1 }, "", nil, background_style)
    
    --title
    local title_style = deepcopy(empty_style)
    title_style.text_color = {r=0,g=1,b=0,a=1}
    title_style.text_size = 0.2
    menu_objects.title = create_ui_element(menu_objects.base.object_ptr, ui_types.common,{ x = 0.5, y = 0.85 }, { x = 2, y = 2 }, "HMTF", nil, title_style)

    --start_button
    local start_style = deepcopy(empty_style)
    start_style.text_color = {r=1,g=1,b=0,a=1}
    start_style.text_size = 0.1
    menu_objects.start_text = create_ui_element(menu_objects.base.object_ptr, ui_types.common,{ x = 0.5, y = 0.5 }, { x = 0.2, y = 0.2 }, "start", nil, start_style)

        
    menu_objects.start_button = create_ui_element(menu_objects.base.object_ptr, ui_types.common,{ x = 0.5, y = 0.5 }, { x = 0.2, y = 0.2 }, ">         <", nil, {deepcopy(empty_style),start_style,start_style})

    menus_locations.title = 0
end

function start_all_menus()
    local base_x_location = 0

    
    --base
    local base_style = advanced_ui_style()
    
    menu_objects.base = create_ui_element(this_object.object_ptr, ui_types.common, { x = 0 + base_x_location, y = 0 },{ x = 0, y = 0 }, "", nil, deepcopy(empty_style))

    start_main_menu()

end

function START()
    global_data:set_var("pause", 1)
    time:set_speed(0)

    local layers = global_data:get_var("layers")

    this_object = game_object(this_object_ptr)

    start_all_menus()
    --show_pause_menu(true)
end

function UPDATE()
    --[[
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
    ]]
end

function END()
    time:set_speed(1)
    global_data:set_var("pause", 0)
end

function COLLIDE(collision_info)
end
