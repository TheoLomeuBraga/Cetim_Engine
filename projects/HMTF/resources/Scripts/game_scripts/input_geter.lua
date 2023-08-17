require("engine_libs.objects.global_data")
require("engine_libs.objects.input")
require("engine_libs.functions")

inputs_last_frame = {}

inputs = {
    foward = 0,
    left = 0,
    jump = false,
    action_1 = false,
    action_2 = false,
    mouse_view_x = 0,
    mouse_view_y = 0,
    analog_view_x = 0,
    analog_view_y = 0,
    menu = false,
}



function START()
    inputs_last_frame = deepcopy(inputs)
end

function UPDATE()

    inputs = {
        foward = keys_axis:get_input(input_devices.keyboard,"w") - keys_axis:get_input(input_devices.keyboard,"s") - keys_axis:get_input_joystick(1,"ry"),
        left = keys_axis:get_input(input_devices.keyboard,"a") - keys_axis:get_input(input_devices.keyboard,"d") - keys_axis:get_input_joystick(1,"rx"),
        jump = keys_axis:get_input(input_devices.keyboard,"space") + keys_axis:get_input_joystick(1,"a"),
        action_1 = keys_axis:get_input(input_devices.mouse,"left") + keys_axis:get_input_joystick(1,"rt_axis"),
        action_2 = keys_axis:get_input(input_devices.mouse,"right") + keys_axis:get_input_joystick(1,"lt_axis"),
        mouse_view_x = keys_axis:get_input(input_devices.mouse,"movement_x"),
        mouse_view_y = keys_axis:get_input(input_devices.mouse,"movement_y"),
        analog_view_x = keys_axis:get_input_joystick(1,"lx"),
        analog_view_y = keys_axis:get_input_joystick(1,"ly"),
        menu = keys_axis:get_input(input_devices.keyboard,"escape") + keys_axis:get_input_joystick(1,"start"),
    }

    if inputs.foward < -1 then
        inputs.foward = -1
    elseif inputs.foward > 1 then
        inputs.foward = 1
    end

    if inputs.left < -1 then
        inputs.left = -1
    elseif inputs.left > 1 then
        inputs.left = 1
    end
    

    
    global_data:set("inputs",inputs)
    global_data:set("inputs_last_frame",inputs_last_frame)
    inputs_last_frame = deepcopy(inputs)
end

function COLLIDE(collision_info)
end

function END()
end
