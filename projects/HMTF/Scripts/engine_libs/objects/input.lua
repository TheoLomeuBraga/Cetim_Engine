register_function_set("input")

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








keys_axis = {
    [input_devices.keyboard] = {},
    [input_devices.joystick] = {},
    [input_devices.mouse] = {},
}



function set_cursor_position(x, y)
    c_set_cursor_position(x, y)
end

function get_text_input()
    return c_get_keyboard_text_input()
end

function set_text_input_geter(on)
    c_set_keyboard_text_input(on)
end

function keys_axis:get_input(device, key)
    return c_get_input(device, 0, key)
end

function get_keyboard_input(key)
    return c_get_input(1, 0, key)
end

function get_mouse_input(key)
    return c_get_input(3, 0, key)
end

function get_input_joystick(joystick_no, key)
    return c_get_input(input_devices.joystick, joystick_no, key)
end

function set_joystick_vibration(joystick_no,left,right,time)
    c_set_joystick_vibration(joystick_no,left,right,time)
end

function set_joystick_led(joystick_no,r,g,b)
    c_set_joystick_led(joystick_no,r,g,b)
end

function  enable_cursor(on)
    c_enable_cursor(on)
end
