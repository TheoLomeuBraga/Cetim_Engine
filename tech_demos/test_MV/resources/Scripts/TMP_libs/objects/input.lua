input_devices = {
    keyboard = 0,
    joystick = 1,
    mouse = 2,
    keyboard_text = 3,
}

key_axis_dictionarys = {
    [input_devices.keyboard] = {
        [0] = 48,
        ["0"] = 48,
        [1] = 49,
        ["1"] = 49,
        [2] = 50,
        ["2"] = 50,
        [3] = 51,
        ["3"] = 51,
        [4] = 52,
        ["4"] = 52,
        [5] = 53,
        ["5"] = 53,
        [6] = 54,
        ["6"] = 54,
        [7] = 55,
        ["7"] = 55,
        [8] = 56,
        ["8"] = 56,
        [9] = 57,
        ["9"] = 57,

    },
    [input_devices.joystick] = {
        x = 0,
        a = 1,
        b = 2,
        y = 3,
        lb = 4,
        rb = 5,
        back = 6,
        start = 7,
        lt = 9,
        rt = 10,
        up = 11,
        down = 12,
        left = 13,
        right = 14,
        rx = "axis_0",
        ry = "axis_1",
        lx = "axis_2",
        ly = "axis_3",
        rt_axis = "axis_4",
        lt_axis = "axis_5"
    },
    [input_devices.mouse] = {{
        left = 1,
        right = 2,
        scroll_button = 3,
        x = "x",
        y = "y",
        normalized_x = "normalized_x",
        normalized_y = "normalized_y",
        scroll = "scroll",
    }},
}



function set_cursor_position(x,y)
end
function get_keyboard_input()
end
function get_mouse_input()
end
function get_joystick_input()
end
function set_keyboard_text_input(on)
end
function get_keyboard_text_input()
end
function get_inputs(device)
end

keys_axis = {
    [input_devices.keyboard] = {},
    [input_devices.joystick] = {},
    [input_devices.mouse] = {},
    [input_devices.keyboard_text] = "",
}
function keys_axis:get_inputs()
    keys_axis[input_devices.keyboard] = get_keyboard_input()
    keys_axis[input_devices.joystick] = get_joystick_input()
    keys_axis[input_devices.mouse] = get_mouse_input()
    keys_axis[input_devices.keyboard_text] = get_keyboard_text_input()
end
function keys_axis:set_keyboard_text_input_geter(on)
    set_keyboard_text_input(on)
end
function keys_axis:get_input_from_device(device,key)
    key_translation = key_axis_dictionarys[device][key]
    return keys_axis[device][key_translation]
end