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

local key_axis_dictionarys = {
    [input_devices.keyboard] = {
        [0] = 48,["0"] = 48,
        [1] = 49,["1"] = 49,
        [2] = 50,["2"] = 50,
        [3] = 51,["3"] = 51,
        [4] = 52,["4"] = 52,
        [5] = 53,["5"] = 53,
        [6] = 54,["6"] = 54,
        [7] = 55,["7"] = 55,
        [8] = 56,["8"] = 56,
        [9] = 57,["9"] = 57,
        space = 32,
        a = 65,
        b = 66,
        c = 67,
        d = 68,
        e = 69,
        f = 70,
        g = 71,
        h = 72,
        i = 73,
        j = 74,
        k = 75,
        l = 76,
        m = 77,
        n = 78,
        o = 79,
        p = 80,
        q = 81,
        r = 82,
        s = 83,
        t = 84,
        u = 85,
        v = 86,
        w = 87,
        x = 88,
        y = 89,
        z = 90,
        backslash = 92,
        escape = 256,
        enter = 257,
        tab = 258,
        backspace = 259,
        insert = 260,
        delete = 261,
        right = 262,
        left = 263,
        down = 264,
        up = 265,
        caps_locl = 280,
        shift = 340,
        ctrl = 341,
        alt = 342,
        right_shift = 344,
        right_ctrl = 345,
        right_alt = 346,
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
    [input_devices.mouse] = {
        left = 0,
        right = 1,
        scroll_button = 2,
        x = "x",
        y = "y",
        normalized_x = "normalized_x",
        normalized_y = "normalized_y",
        scroll = "scroll",
    },
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
    return get_input(device, 0, key_axis_dictionarys[device][key])
end

function keys_axis:get_input_joystick(joystick_no, key)
    return get_input(input_devices.joystick, joystick_no, key_axis_dictionarys[input_devices.joystick][key])
end
