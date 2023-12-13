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

local arow_style = deepcopy(empty_style)
arow_style.text_color = { r = 1, g = 1, b = 0, a = 1 }
arow_style.text_size = 0.1

in_main_menu = 0

menu_types = {
    title = "title",
    start = "start",
    config = "config",
    pause = "pause",
}
menu_selectred = "pause"
local menu_objects = {}
local menus_locations = {
    title = 0,
    start = 1,
}

function select_menu(pos)
    menu_objects.base.components.ui_component.position.x = menus_locations[pos]
    menu_objects.base.components.ui_component:set()
end

function go_to_start_menu(state, id)
    if state == "click" then
        select_menu(menu_types.start)
    end
end

function go_to_title_menu(state, id)
    if state == "click" then
        select_menu(menu_types.title)
    end
end





function start_title_menu()
    --title
    local title_style = deepcopy(empty_style)
    title_style.text_color = { r = 0, g = 1, b = 0, a = 1 }
    title_style.text_size = 0.2
    menu_objects.title = create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = 0.5, y = 0.85 },
        { x = 2, y = 2 }, "HMTF", nil, title_style)

    --start_button
    local start_style = deepcopy(empty_style)
    start_style.text_color = { r = 1, g = 1, b = 0, a = 1 }
    start_style.text_size = 0.1
    menu_objects.start_text = create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = 0.5, y = 0.5 },
        { x = 0.2, y = 0.2 }, "start", nil, start_style)

    create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = 0.5, y = 0.5 },{ x = 0.25, y = 0.2 },  "start", "go_to_start_menu", start_style,arow_style)

    
end

function start_start_menu()
    local button = deepcopy(empty_style)
    
    button.text_size = 0.1

    button.text_color = { r = 1, g = 1, b = 0, a = 1 }
    create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -0.5, y = 0.8 },{ x = 0.5, y = 0.15 }, "play", "go_to_title_menu", button,arow_style)

    button.text_color = { r = 0, g = 1, b = 1, a = 1 }
    create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -0.5, y = 0.5 },{ x = 0.5, y = 0.15 }, "config", "go_to_title_menu", button,arow_style)

    button.text_color = { r = 1, g = 0, b = 0, a = 1 }
    create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -0.5, y = 0.2 },{ x = 0.5, y = 0.15 }, "back", "go_to_title_menu", button,arow_style)

end

function start_all_menus()
    local base_x_location = 0

    if in_main_menu > 0 then
        --background
        local background_style = deepcopy(empty_style)
        background_style.background_color = { r = 0, g = 0.2, b = 0.2, a = 1 }
        background_style.text_color = { r = 0, g = 1, b = 0, a = 1 }
        background_style.background_image = "resources/Textures/null.png"
        create_ui_element(this_object.object_ptr, ui_types.common, { x = 0.5, y = 0.5 },{ x = 1, y = 1 }, "", nil, background_style)
    end


    --base
    local base_style = advanced_ui_style()

    menu_objects.base = create_ui_element(this_object.object_ptr, ui_types.common, { x = 0, y = 0 }, { x = 0, y = 0 }, "",
        nil, deepcopy(empty_style))

    start_title_menu()
    start_start_menu()
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

end

function END()
    time:set_speed(1)
    global_data:set_var("pause", 0)
end

function COLLIDE(collision_info)
end
