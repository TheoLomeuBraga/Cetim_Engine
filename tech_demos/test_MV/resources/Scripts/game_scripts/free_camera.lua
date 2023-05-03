require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.short_cuts.create_camera")
require("TMP_libs.objects.input")
require("TMP_libs.objects.time")
require("TMP_libs.objects.window")

require("math")

this_object = {}

base_sensivity = 120
base_speed = 7

local mouse_move = { x = 0, y = 0 }
local mouse_move_this_frame = { x = 0, y = 0 }
local mouse_move_last_frame = { x = 0, y = 0 }

local current_pos = { x = 0, y = 0,z = 0 }
local current_rot = { x = 0, y = 0 }

function START()
    this_object = game_object:new(this_object_ptr)

    this_object.components[components.transform]:get()
    current_pos = deepcopy(this_object.components[components.transform].position)
    current_rot = { x = this_object.components[components.transform].rotation.x, y = this_object.components[components.transform].rotation.y }
end

function run_movement()

    local speed = base_speed * Time.delta

    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.w]) == 1 then
        current_pos.x = current_pos.x + (1 * speed)
    end
    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.s]) == 1 then
        current_pos.x = current_pos.x - (1 * speed)
    end

    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.a]) == 1 then
        current_pos.z = current_pos.z - (1 * speed)
    end
    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.d]) == 1 then
        current_pos.z = current_pos.z + (1 * speed)
    end

    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.space]) == 1 then
        current_pos.y = current_pos.y + (1 * speed)
    end
    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.ctrl]) == 1 then
        current_pos.y = current_pos.y - (1 * speed)
    end

    this_object.components[components.transform]:change_position(current_pos.x,current_pos.y,current_pos.z)

end

function run_rotation()
    mouse_move_this_frame = {
        x = keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.normalized_x]),
        y = keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.normalized_y]),
    }

    mouse_move = {
        x = (mouse_move_last_frame.x - mouse_move_this_frame.x),
        y = (mouse_move_last_frame.y - mouse_move_this_frame.y),
    }

    if keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.scroll_button]) == 1 then
        
        
        current_rot.x = current_rot.x + (mouse_move.x*base_sensivity)
        current_rot.y = current_rot.y + (mouse_move.y*base_sensivity)

        --current_rot.y = math.max(-90,math.min(90,current_rot.y))
    
        this_object.components[components.transform]:change_rotation(current_rot.y, current_rot.x,0)
        
        print("mouse_move: ", current_rot.x, current_rot.y)
    end
    mouse_move_last_frame = deepcopy(mouse_move_this_frame)
end

function UPDATE()run_rotation()
    Time:get()
    

    run_movement()
    run_rotation()
    
    
end

function COLLIDE(collision_info)
end

function END()
end
