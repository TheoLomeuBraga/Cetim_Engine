input_devices = {
    keyboard = 1,
    joystick = 2,
    mouse = 3,
}

input_keys = {
    keyboard = {
        [0] = 0,
        ["0"] = 0,
        [1] = 1,
        ["1"] = 1,
        [2] = 2,
        ["2"] = 2,
        [3] = 3,
        ["3"] = 3,
        [4] = 4,
        ["4"] = 4,
        [5] = 5,
        ["5"] = 5,
        [6] = 6,
        ["6"] = 6,
        [7] = 7,
        ["7"] = 7,
        [8] = 8,
        ["8"] = 8,
        [9] = 9,
        ["9"] = 9,
        space = "space",
        a = "a",
        b = "b",
        c = "c",
        d = "d",
        e = "e",
        f = "f",
        g = "g",
        h = "h",
        i = "i",
        j = "j",
        k = "k",
        l = "l",
        m = "m",
        n = "n",
        o = "o",
        p = "p",
        q = "q",
        r = "r",
        s = "s",
        t = "t",
        u = "u",
        v = "v",
        w = "w",
        x = "x",
        y = "y",
        z = "z",
        backslash = "backslash",
        escape = "escape",
        enter = "enter",
        tab = "tab",
        backspace = "backspace",
        insert = "insert",
        delete = "delete",
        right = "right",
        left = "left",
        down = "down",
        up = "up",
        caps_locl = "caps_locl",
        shift = "shift",
        ctrl = "ctrl",
        alt = "alt",
        right_shift = "right_shift",
        right_ctrl = "right_ctrl",
        right_alt = "right_alt",
    },
    mouse = {
        left = "left",
        right = "right",
        scroll_button = "scroll_button",
        x = "x",
        y = "y",
        normalized_x = "normalized_x",
        normalized_y = "normalized_y",
        movement_x = "movement_x",
        movement_y = "movement_y",
        scroll = "scroll",
    },
    joystick = {
        x = "x",
        a = "a",
        b = "b",
        y = "y",
        lb = "lb",
        rb = "rb",
        back = "back",
        start = "start",
        lt = "lt",
        rt = "rt",
        up = "up",
        down = "down",
        left = "left",
        right = "right",
        rx = "rx",
        ry = "ry",
        lx = "lx",
        ly = "ly",
        rt_axis = "rt_axis",
        lt_axis = "lt_axis"
    }
}






function set_cursor_position(x, y)
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

function get_last_device_used()
end

--[[
function get_inputs(device)
end
]]
function get_input(device, joystick_no, key)
end

keys_axis = {
    [input_devices.keyboard] = {},
    [input_devices.joystick] = {},
    [input_devices.mouse] = {},
}



function keys_axis:set_cursor_position(x, y)
    set_cursor_position(x, y)
end

function keys_axis:get_text_input()
    return get_keyboard_text_input()
end

function keys_axis:set_text_input_geter(on)
    set_keyboard_text_input(on)
end

function keys_axis:get_input(device, key)
    --return get_input(device, 0, key_axis_dictionarys[device][key])
    return get_input(device, 0, key)
end

function keys_axis:get_input_joystick(joystick_no, key)
    --return get_input(input_devices.joystick, joystick_no, key_axis_dictionarys[input_devices.joystick][key])
    return get_input(input_devices.joystick, joystick_no, key)
end
