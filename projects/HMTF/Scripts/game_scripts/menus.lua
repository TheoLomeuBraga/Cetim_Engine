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

require("function_sets.simple_ui")


local config_path = get_config_folder_path() .. "/hmtf"
local config_file_path = config_path .. "/configs_save.lua"

local arow_style = deepcopy(empty_style)
arow_style.text_color = { r = 1, g = 1, b = 0, a = 1 }
arow_style.text_size = 0.1

in_main_menu = 0

local ui_selection_max_min = { 1, 1 }

menu_types = {
    title = "title",
    start = "start",
    config = "config",
    play = "play",
    pause = "pause",
}
menu_selected = "pause"
local menu_objects = {}
local menus_locations = {
    title = 0,
    start = 1,
    config = 2,
    play = 3,
}

function select_menu(pos)
    menu_objects.base.components.ui_component.position.x = menus_locations[pos]
    menu_objects.base.components.ui_component:set()
end

function save_configs()
    window:get()
    local configs = {
        volume = get_set_global_volume(),
        mouse_sensitivity = global_data.mouse_sensitivity,
        full_screen = window.full_screen
    }


    create_directory(config_path)
    serializer.save_table(config_file_path, configs)
end

--button functions

local going_to_main_menu = false
function go_to_main_menu(state, id)
    if state == "click" then
        remove_object(this_object_ptr)
        game_object(global_data.core_object_ptr).components.lua_scripts.scripts["core"].functions.load_sceane_request({
            "main_menu" })
        going_to_main_menu = true
    end
end

function quit_game(state, id)
    if state == "click" then
        window:close()
    end
end

function resume_game(state, id)
    if state == "click" then
        remove_object(this_object_ptr)
    end
end

function go_to_start_menu(state, id)
    if state == "click" then
        select_menu(menu_types.start)
        save_configs()
        ui_selection_max_min = { 2, 4 }
        global_data.ui_selection_id = 2
    end
end

function go_to_load_menu(state, id)
    if state == "click" then
    end
end

function go_to_title_menu(state, id)
    if state == "click" then
        select_menu(menu_types.title)
        ui_selection_max_min = { 1, 1 }
        global_data.ui_selection_id = 1
    end
end

function go_to_config_menu(state, id)
    if state == "click" then
        select_menu(menu_types.config)
        ui_selection_max_min = { 5, 8 }
        global_data.ui_selection_id = 5
    end
end

function go_to_play_menu(state, id)
    if state == "click" then
        select_menu(menu_types.play)
        ui_selection_max_min = { 9, 11 }
        global_data.ui_selection_id = 9
    end
end

function new_game(state, id)
    if state == "click" then
        print("new_game")
        core_obj = game_object(global_data.core_object_ptr)
        --core_obj.components.lua_scripts.scripts["core"].functions.load_sceane_request({ "floating_island_hub" })
        core_obj.components.lua_scripts.scripts["core"].functions.load_sceane_request({ "hub" })
    end
end

function toogle_full_screen(state, id)
    if state == "click" then
        window:get()
        window.full_screen = not window.full_screen
        window.resolution.x = 256 * 4
        window.resolution.y = 224 * 4
        window:set()
        save_configs()
    end
end

sensitivity_display = {}
function sensitivity_slider(state, id)
    if state == "hold" then
        local drag_obj = game_object(id)
        local pos = drag_obj.components.ui_component.position

        pos.x = pos.x + global_data.inputs.mouse_view_x
        pos.x = math.min(0.8 - 2, math.max(0.2 - 2, pos.x))

        local sensitivity = math.floor((pos.x + 1.8) * 168) * 0.3
        sensitivity_display.components.ui_component.text = "sensitivity: " .. sensitivity
        global_data.mouse_sensitivity = sensitivity

        sensitivity_display.components.ui_component:set()
        drag_obj.components.ui_component:set()
    elseif state == "hover" then
        local drag_obj = game_object(id)
        local pos = drag_obj.components.ui_component.position

        time:get()
        pos.x = pos.x + (-global_data.inputs.left * time.delta * 0.5)
        pos.x = math.min(0.8 - 2, math.max(0.2 - 2, pos.x))

        local sensitivity = math.floor((pos.x + 1.8) * 168) * 0.3
        sensitivity_display.components.ui_component.text = "sensitivity: " .. sensitivity
        global_data.mouse_sensitivity = sensitivity

        sensitivity_display.components.ui_component:set()
        drag_obj.components.ui_component:set()
    end
end

volume_display = {}
function volume_slider(state, id)
    if state == "hold" then
        local drag_obj = game_object(id)
        local pos = drag_obj.components.ui_component.position
        pos.x = pos.x + global_data.inputs.mouse_view_x
        pos.x = math.min(0.8 - 2, math.max(0.2 - 2, pos.x))

        local volume = math.floor((pos.x + 1.8) * 168)
        volume_display.components.ui_component.text = "volume: " .. volume .. "%"

        get_set_global_volume(volume)

        volume_display.components.ui_component:set()
        drag_obj.components.ui_component:set()
    elseif state == "hover" then
        local drag_obj = game_object(id)
        local pos = drag_obj.components.ui_component.position
        pos.x = pos.x + (-global_data.inputs.left * time.delta * 0.5)
        pos.x = math.min(0.8 - 2, math.max(0.2 - 2, pos.x))

        local volume = math.floor((pos.x + 1.8) * 168)
        volume_display.components.ui_component.text = "volume: " .. volume .. "%"

        get_set_global_volume(volume)

        volume_display.components.ui_component:set()
        drag_obj.components.ui_component:set()
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

    create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = 0.5, y = 0.5 },
        { x = 0.25, y = 0.2 }, "start", "go_to_start_menu", start_style, arow_style, 1)
end

function start_start_menu()
    local button = deepcopy(empty_style)

    button.text_size = 0.1

    if in_main_menu > 0 then
        button.text_color = { r = 1, g = 1, b = 0, a = 1 }
        create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -0.5, y = 0.8 },
            { x = 0.5, y = 0.15 }, "play", "go_to_play_menu", button, arow_style, 2)

        button.text_color = { r = 0, g = 1, b = 1, a = 1 }
        create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -0.5, y = 0.5 },
            { x = 0.5, y = 0.15 }, "config", "go_to_config_menu", button, arow_style, 3)

        button.text_color = { r = 1, g = 0, b = 0, a = 1 }
        create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -0.5, y = 0.2 },
            { x = 0.5, y = 0.15 }, "exit", "quit_game", button, arow_style, 4)
    else
        button.text_color = { r = 0, g = 1, b = 1, a = 1 }
        create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -0.5, y = 0.8 },
            { x = 0.5, y = 0.15 }, "resume", "resume_game", button, arow_style, 2)


        button.text_color = { r = 1, g = 1, b = 0, a = 1 }
        create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -0.5, y = 0.5 },
            { x = 0.5, y = 0.15 }, "config", "go_to_config_menu", button, arow_style, 3)

        button.text_color = { r = 1, g = 0, b = 0, a = 1 }
        create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -0.5, y = 0.2 },
            { x = 0.5, y = 0.15 }, "main menu", "go_to_main_menu", button, arow_style, 4)
    end
end

function start_config_menu()
    --title
    local title_style = deepcopy(empty_style)
    title_style.text_color = { r = 1, g = 1, b = 0, a = 1 }
    title_style.text_size = 0.1
    create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = -1.5, y = 0.85 }, { x = 2, y = 2 }, "config",
        nil, title_style)

    exit_style = deepcopy(title_style)
    exit_style.text_color = { r = 1, g = 0, b = 0, a = 1 }
    exit_hover_style = deepcopy(exit_style)
    exit_hover_style.background_color = { r = 1, g = 0.5, b = 0.5, a = 1 }
    create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = -1.8, y = 0.85 }, { x = 0.1, y = 0.1 }, "<",
        "go_to_start_menu", { exit_style, exit_hover_style }, 5)

    title_style.text_color = { r = 1, g = 1, b = 1, a = 1 }

    --drag button test
    --create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = -1.5, y = 0.5 },{ x = 0.1, y = 0.1 }, "", "drag_test", exit_hover_style)



    --add sensitivity control

    local configs = serializer.load_table(config_file_path)
    if configs == nil then
        save_configs()
        configs = serializer.load_table(config_file_path)
    end

    window:get()
    window.full_screen = configs.full_screen
    window:set()

    sensitivity_display = create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = -1.5, y = 0.6 },
        { x = 0.5, y = 0.17 }, "sensitivity: " .. configs.mouse_sensitivity, nil, title_style)

    local sensitivity_slider_pos = ((configs.mouse_sensitivity / 30) * 0.6) + 0.2
    create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = sensitivity_slider_pos - 2, y = 0.55 },
        { x = 0.2, y = 0.15 }, "^", "sensitivity_slider", { title_style, arow_style, arow_style }, 6)

    local slider_bar = deepcopy(title_style)
    slider_bar.background_color = { r = 1, g = 1, b = 0, a = 1 }
    create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = -1.5, y = 0.58 }, { x = 0.6, y = 0.005 }, "",
        nil, slider_bar)

    --add volume control
    volume_display = create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = -1.5, y = 0.3 },
        { x = 0.5, y = 0.17 }, "volume: " .. configs.volume .. "%", nil, title_style)

    local volume_slider_pos = ((configs.volume / 100) * 0.6) + 0.2
    --local volume_slider_pos = 0.5
    create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = volume_slider_pos - 2, y = 0.25 },
        { x = 0.2, y = 0.15 }, "^", "volume_slider", { title_style, arow_style, arow_style }, 7)

    local slider_bar = deepcopy(title_style)
    slider_bar.background_color = { r = 1, g = 1, b = 0, a = 1 }
    create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = -1.5, y = 0.28 }, { x = 0.6, y = 0.005 }, "",
        nil, slider_bar)

    --toogle_full_screen
    local button = deepcopy(title_style)
    button.text_size = 0.06
    create_ui_element(menu_objects.base.object_ptr, ui_types.common, { x = -1.5, y = 0.1 },
        { x = 0.5, y = 0.17 }, "toogle full screen", "toogle_full_screen", { title_style, arow_style, arow_style }, 8)
end

function start_play_menu()
    local button = deepcopy(empty_style)

    button.text_size = 0.1

    button.text_color = { r = 0.2, g = 1, b = 0.2, a = 1 }
    create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -2.5, y = 0.8 },
        { x = 0.5, y = 0.15 }, "new game", "new_game", button, arow_style, 9)

    button.text_color = { r = 0, g = 1, b = 1, a = 1 }
    create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -2.5, y = 0.5 },
        { x = 0.5, y = 0.15 }, "load game", "go_to_load_menu", button, arow_style, 10)

    button.text_color = { r = 1, g = 0, b = 0, a = 1 }
    create_ui_element_with_arows(menu_objects.base.object_ptr, ui_types.common, { x = -2.5, y = 0.2 },
        { x = 0.5, y = 0.15 }, "back", "go_to_start_menu", button, arow_style, 11)
end

function start_all_menus()
    local base_x_location = 0

    if in_main_menu > 0 then
        --background
        local background_style = deepcopy(empty_style)
        background_style.background_color = { r = 0, g = 0.2, b = 0.2, a = 1 }
        background_style.text_color = { r = 0, g = 1, b = 0, a = 1 }
        background_style.background_image = "Textures/null.png"
        create_ui_element(this_object.object_ptr, ui_types.common, { x = 0.5, y = 0.5 }, { x = 1, y = 1 }, "", nil,
            background_style)
    else
        --background
        local background_style = deepcopy(empty_style)
        background_style.background_color = { r = 0.25, g = 0.25, b = 0.25, a = 0.75 }
        background_style.text_color = { r = 0, g = 1, b = 0, a = 1 }
        background_style.background_image = "Textures/white.png"
        create_ui_element(this_object.object_ptr, ui_types.common, { x = 0.5, y = 0.5 }, { x = 1, y = 1 }, "", nil,
            background_style)
    end


    --base
    local base_style = advanced_ui_style()

    menu_objects.base = create_ui_element(this_object.object_ptr, ui_types.common, { x = 0, y = 0 }, { x = 0, y = 0 }, "",
        nil, deepcopy(empty_style))

    start_title_menu()
    start_start_menu()
    start_config_menu()
    start_play_menu()

    if in_main_menu < 1 then
        select_menu(menu_types.start)
        ui_selection_max_min = { 2, 4 }
        global_data.ui_selection_id = 2
    end
end

function START()
    global_data.pause = 1
    time:set_speed(0)

    local layers = global_data.layers

    this_object = game_object(this_object_ptr)

    start_all_menus()

    global_data.ui_selection_id = 1

    
end

function UPDATE()
    if global_data.inputs.menu > 0 and global_data.inputs_last_frame.menu < 1 and in_main_menu < 1 then
        remove_object(this_object_ptr)
    end

    if global_data.inputs.foward == 1 and global_data.inputs_last_frame.foward ~= 1 then
        global_data.ui_selection_id = global_data.ui_selection_id - 1
    end

    if global_data.inputs.foward == -1 and global_data.inputs_last_frame.foward ~= -1 then
        global_data.ui_selection_id = global_data.ui_selection_id + 1
    end

    if global_data.ui_selection_id < ui_selection_max_min[1] then
        global_data.ui_selection_id = ui_selection_max_min[2]
    elseif global_data.ui_selection_id > ui_selection_max_min[2] then
        global_data.ui_selection_id = ui_selection_max_min[1]
    end
end

function END()
    if not going_to_main_menu then
        time:set_speed(1)
        
        global_data.pause = 0
    end
    save_configs()
    global_data.open_pause_menu_ptr = nil
end

function COLLIDE(collision_info)
end
