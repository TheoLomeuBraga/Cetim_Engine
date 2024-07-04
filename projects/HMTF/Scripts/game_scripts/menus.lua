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

in_main_menu = 0

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

function START()
    global_data.pause = 1
    time:set_speed(0)

    local configs = serializer.load_table(config_file_path)
    if configs == nil then
        configs = serializer.load_table(config_file_path)
    end

    global_data.mouse_sensitivity = configs.mouse_sensitivity
    get_set_global_volume(configs.volume)
    window:get()
    window.full_screen = configs.full_screen
    window:set()
end

local joystick_selector_limits = { 1, 1 }
function update_ui()
    if global_data.ui_selection_id == nil then
        global_data.ui_selection_id = -1
    end

    if get_main_input_device() == 1 then
        if global_data.inputs.foward >= 1 and global_data.inputs_last_frame.foward < 1 then
            global_data.ui_selection_id = global_data.ui_selection_id - 1
        elseif global_data.inputs.foward <= -1 and global_data.inputs_last_frame.foward > -1 then
            global_data.ui_selection_id = global_data.ui_selection_id + 1
        end
        if global_data.ui_selection_id < joystick_selector_limits[1] then
            global_data.ui_selection_id = joystick_selector_limits[1]
        elseif global_data.ui_selection_id > joystick_selector_limits[2] then
            global_data.ui_selection_id = joystick_selector_limits[2]
        end
    else
        global_data.ui_selection_id = -1
    end



    if global_data.inputs ~= nil then
        local inputs = global_data.inputs
        local inputs_last_frame = global_data.inputs_last_frame

        local cursor_movement = {
            x = inputs.mouse_pos_x - cursor_pos_last_frame.x,
            y = cursor_pos_last_frame.y -
                inputs.mouse_pos_y
        }
        set_selection_state({ x = inputs.mouse_pos_x, y = inputs.mouse_pos_y }, cursor_movement,
            global_data.ui_selection_id, inputs.select)
        cursor_pos_last_frame = { x = inputs.mouse_pos_x, y = inputs.mouse_pos_y }
    end
end

local selected_menu = 0

local style = simple_ui_style({ r = 0, g = 0, b = 0, a = 0 }, 0.0, { r = 0, g = 0, b = 0, a = 0 },
    { r = 1, g = 1, b = 1, a = 1 })
local style_howver = simple_ui_style({ r = 0, g = 0, b = 0, a = 0 }, 0.0, { r = 0, g = 0, b = 0, a = 0 },
    { r = 0.5, g = 0.5, b = 1, a = 1 })
local style_click = simple_ui_style({ r = 0, g = 0, b = 0, a = 0 }, 0.0, { r = 0, g = 0, b = 0, a = 0 },
    { r = 0, g = 0, b = 1, a = 1 })



menu_ativado = true

function UPDATE()
    update_ui()

    if selected_menu == 0 and in_main_menu == 0 then
        selected_menu = 1
    end

    if in_main_menu == 0 and global_data.inputs.menu == 1 and global_data.inputs_last_frame.menu == 0 then
        menu_ativado = not menu_ativado
    end

    if menu_ativado then
        global_data.pause = 1
        time:set_speed(0)
    else
        global_data.pause = 0
        time:set_speed(1)
    end

    if menu_ativado then
        if selected_menu == 0 then
            joystick_selector_limits = { 1, 1 }


            local start_tf = simple_ui_transform()

            start_tf.scale = { x = 0.5, y = 0.5, z = 0.5 }

            if simple_ui_button(start_tf, "start", style, 1, style_howver) == 1 then
                selected_menu = 1
            end
        elseif selected_menu == 1 then
            if global_data.inputs.back == 1 and global_data.inputs_last_frame.back == 0 then
                selected_menu = 0
            end

            joystick_selector_limits = { 1, 3 }


            local start_tf = simple_ui_transform()
            start_tf.scale = { x = 0.5, y = 0.25, z = 0.5 }
            start_tf.position = { x = 0, y = 0.25, z = 0 }
            if simple_ui_button(deepcopy(start_tf), "start", style, 1, style_howver) == 1 then
                print("new_game")
                core_obj = game_object(global_data.core_object_ptr)
                core_obj.components.lua_scripts.scripts["core"].functions.load_sceane_request({ "floating_island_hub" })
                --core_obj.components.lua_scripts.scripts["core"].functions.load_sceane_request({ "hub" })
            end

            start_tf.position = { x = 0, y = -0.25, z = 0 }
            if simple_ui_button(deepcopy(start_tf), "config", style, 2, style_howver) == 1 then
                selected_menu = 2
            end

            start_tf.position = { x = 0, y = -0.75, z = 0 }
            if simple_ui_button(deepcopy(start_tf), "exit", style, 3, style_howver) == 1 then
                if in_main_menu == 1 then
                    window:close()
                else
                    game_object(global_data.core_object_ptr).components.lua_scripts.scripts["core"].functions
                        .load_sceane_request({ "main_menu" })
                    remove_object(this_object_ptr)
                end
            end
        elseif selected_menu == 2 then
            if global_data.inputs.back == 1 and global_data.inputs_last_frame.back == 0 then
                save_configs()
                selected_menu = 1
            end

            joystick_selector_limits = { 1, 4 }

            --sound
            local start_tf = simple_ui_transform()
            start_tf.scale = { x = 0.5, y = 0.25, z = 0.5 }
            start_tf.position = { x = 0, y = 0.25, z = 0 }
            if simple_ui_button(deepcopy(start_tf), "sensitivity: " .. tostring(global_data.mouse_sensitivity), style, 2, style_howver) == 0.5 then
                if global_data.inputs.left >= 1 and global_data.inputs_last_frame.left < 1 then
                    global_data.mouse_sensitivity = (math.max(global_data.mouse_sensitivity - 1, 0))
                elseif global_data.inputs.left <= -1 and global_data.inputs_last_frame.left > -1 then
                    global_data.mouse_sensitivity = (math.min(global_data.mouse_sensitivity + 1, 40))
                end
            end
            start_tf.position = { x = 0.75, y = 0.25, z = 0 }
            start_tf.scale = { x = 0.25, y = 0.25, z = 0.25 }
            if simple_ui_button(deepcopy(start_tf), ">", style, 4, style_howver) == 1 then
                global_data.mouse_sensitivity = (math.min(global_data.mouse_sensitivity + 1, 40))
            end
            start_tf.position = { x = -0.75, y = 0.25, z = 0 }
            if simple_ui_button(deepcopy(start_tf), "<", style, 4, style_howver) == 1 then
                global_data.mouse_sensitivity = (math.max(global_data.mouse_sensitivity - 1, 0))
            end

            --sensitivity
            start_tf.position = { x = 0, y = -0.25, z = 0 }
            start_tf.scale = { x = 0.5, y = 0.25, z = 0.5 }
            if simple_ui_button(deepcopy(start_tf), "sound: " .. tostring(get_set_global_volume()), style, 3, style_howver) == 0.5 then
                if global_data.inputs.left >= 1 and global_data.inputs_last_frame.left < 1 then
                    get_set_global_volume(math.max(get_set_global_volume() - 5, 0))
                elseif global_data.inputs.left <= -1 and global_data.inputs_last_frame.left > -1 then
                    get_set_global_volume(math.min(get_set_global_volume() + 5, 100))
                end
            end
            start_tf.position = { x = 0.75, y = -0.25, z = 0 }
            start_tf.scale = { x = 0.25, y = 0.25, z = 0.25 }
            if simple_ui_button(deepcopy(start_tf), ">", style, 4, style_howver) == 1 then
                get_set_global_volume(math.min(get_set_global_volume() + 5, 100))
            end
            start_tf.position = { x = -0.75, y = -0.25, z = 0 }
            if simple_ui_button(deepcopy(start_tf), "<", style, 4, style_howver) == 1 then
                get_set_global_volume(math.max(get_set_global_volume() - 5, 0))
            end

            start_tf.position = { x = 0, y = -0.75, z = 0 }
            start_tf.scale = { x = 0.5, y = 0.25, z = 0.5 }
            if simple_ui_button(deepcopy(start_tf), "toogle full screen", style, 4, style_howver) == 1 then
                window:get()
                print(window.full_screen)
                window.full_screen = not window.full_screen
                window.resolution.x = 256 * 4
                window.resolution.y = 224 * 4
                window:set()
                save_configs()
            end
        end
    end
end

function END()
    global_data.pause_menu_obj_ptr = nil
    global_data.open_pause_menu_ptr = nil
    global_data.pause = 0
    time:set_speed(1)
end

function COLLIDE(collision_info)
end
