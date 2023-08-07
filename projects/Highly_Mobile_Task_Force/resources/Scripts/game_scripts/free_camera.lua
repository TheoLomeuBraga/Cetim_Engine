require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.short_cuts.create_camera")
require("TMP_libs.objects.input")
require("TMP_libs.objects.time")
require("TMP_libs.objects.global_data")
require("TMP_libs.objects.window")
require("math")

local this_object = {}

local base_sensivity = 120
local base_speed = 7
local speed_multplier = 4

local mouse_move = { x = 0, y = 0 }
local mouse_move_this_frame = { x = 0, y = 0 }

local current_pos = { x = 0, y = 0, z = 0 }
current_rot = { x = 0, y = 0 }

is_free = false

function START()
    global_data:set_var("camera_object_ptr",this_object_ptr)

    this_object = game_object:new(this_object_ptr)
    
    this_object.components[components.transform]:get()
    current_pos = deepcopy(this_object.components[components.transform].position)
    current_rot = { x = this_object.components[components.transform].rotation.x,
        y = this_object.components[components.transform].rotation.y }
    this_object.components[components.transform]:change_rotation(current_rot.y, current_rot.x, 0)

    
end

function run_rotation()
    window:get()
    local screan_center = {
        x = math.floor(window.resolution.x / 2),
        y = math.floor(window.resolution.y / 2)
    }

    mouse_move_this_frame = {
        x = keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.normalized_x]),
        y = keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.normalized_y]),
    }

    mouse_move = {
        x = mouse_move_this_frame.x - 0.5,
        y = mouse_move_this_frame.y - 0.5,
    }

    --[[
    if keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.scroll_button]) == 1 then
        current_rot.x = current_rot.x - (mouse_move.x * base_sensivity)
        current_rot.y = current_rot.y - (mouse_move.y * base_sensivity)
        current_rot.y = math.max(-90, math.min(90, current_rot.y))
        this_object.components[components.transform]:change_rotation(current_rot.y, current_rot.x, 0)
        keys_axis:set_cursor_position(screan_center.x, screan_center.y)
    end
    ]]
    current_rot.x = current_rot.x - (mouse_move.x * base_sensivity)
    current_rot.y = current_rot.y - (mouse_move.y * base_sensivity)
    current_rot.y = math.max(-90, math.min(90, current_rot.y))
    this_object.components[components.transform]:change_rotation(current_rot.y, current_rot.x, 0)
    this_object.components[components.transform]:get()
    keys_axis:set_cursor_position(screan_center.x, screan_center.y)

end

function run_movement()
    local speed = base_speed * time.delta


    for index, value in ipairs({ 1, 2, 3, 4, 5, 6, 7, 8, 9 }) do
        if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard[value]]) == 1 then
            speed_multplier = value
        end
    end

    speed = speed * speed_multplier

    this_object.components[components.transform]:get()
    current_pos = deepcopy(this_object.components[components.transform].position)

    local additional_pos = { x = 0, y = 0, z = 0 }

    --X
    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.w]) == 1 then
        local dir = this_object.components[components.transform]:get_local_direction(0, 0, 1)
        additional_pos.x = additional_pos.x + (dir.x * speed)
        additional_pos.y = additional_pos.y + (dir.y * speed)
        additional_pos.z = additional_pos.z + (dir.z * speed)
    end
    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.s]) == 1 then
        local dir = this_object.components[components.transform]:get_local_direction(0, 0, -1)
        additional_pos.x = additional_pos.x + (dir.x * speed)
        additional_pos.y = additional_pos.y + (dir.y * speed)
        additional_pos.z = additional_pos.z + (dir.z * speed)
    end

    --Y
    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.a]) == 1 then
        local dir = this_object.components[components.transform]:get_local_direction(1, 0, 0)
        additional_pos.x = additional_pos.x + (dir.x * speed)
        additional_pos.y = additional_pos.y + (dir.y * speed)
        additional_pos.z = additional_pos.z + (dir.z * speed)
    end
    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.d]) == 1 then
        local dir = this_object.components[components.transform]:get_local_direction(-1, 0, 0)
        additional_pos.x = additional_pos.x + (dir.x * speed)
        additional_pos.y = additional_pos.y + (dir.y * speed)
        additional_pos.z = additional_pos.z + (dir.z * speed)
    end

    --Z
    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.space]) == 1 then
        additional_pos.y = additional_pos.y + (1 * speed)
    end
    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.ctrl]) == 1 or keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.c]) == 1 then
        additional_pos.y = additional_pos.y - (1 * speed)
    end

    --move
    this_object.components[components.transform]:change_position(current_pos.x + additional_pos.x,
    current_pos.y + additional_pos.y, current_pos.z + additional_pos.z)
end

local full_screen = false
local zero_pressed_last_frame = 0
function full_screen_toogle()
    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard[0]]) == 0 and zero_pressed_last_frame == 1 then
        full_screen = not full_screen

        window.full_screen = full_screen
        window:set()
    end
    zero_pressed_last_frame = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard[0]])
end


local right_pressed = 0
local right_pressed_last_frame = 0
function get_is_free()
    right_pressed = keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.right])

    if right_pressed == 1 and right_pressed_last_frame == 0 then
        is_free = not is_free
    end

    right_pressed_last_frame = keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.right])
end

function UPDATE()
    time:get()

    get_is_free()

    if is_free then
        run_rotation()
        run_movement()
        full_screen_toogle()
    end
    
end

function COLLIDE(collision_info)
end

function END()
end

