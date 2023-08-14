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
        jump = keys_axis:get_input(input_devices.keyboard,"space") > 0 or keys_axis:get_input_joystick(1,"a"),
        action_1 = keys_axis:get_input(input_devices.mouse,"left") > 0 or keys_axis:get_input_joystick(1,"rt_axis") > 0,
        action_2 = keys_axis:get_input(input_devices.mouse,"right") > 0 or keys_axis:get_input_joystick(1,"lt_axis") > 0,
        mouse_view_x = keys_axis:get_input(input_devices.mouse,"normalized_x"),
        mouse_view_y = keys_axis:get_input(input_devices.mouse,"normalized_y"),
        analog_view_x = keys_axis:get_input_joystick(1,"lx"),
        analog_view_y = keys_axis:get_input_joystick(1,"ly"),
        menu = keys_axis:get_input(input_devices.keyboard,"escape") > 0 or keys_axis:get_input_joystick(1,"start") > 0,
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

    --deepprint(inputs)

    --[[
    print("rx",keys_axis:get_input_joystick(1,"rx"))
    print("ry",keys_axis:get_input_joystick(1,"ry"))

    print("lx",keys_axis:get_input_joystick(1,"lx"))
    print("ly",keys_axis:get_input_joystick(1,"ly"))

    print("rt_axis",keys_axis:get_input_joystick(1,"rt_axis"))
    print("lt_axis",keys_axis:get_input_joystick(1,"lt_axis"))
    ]]
    

    global_data:set_var("inputs_last_frame",inputs_last_frame)
    global_data:set_var("inputs",inputs)
    inputs_last_frame = deepcopy(inputs)
end

function COLLIDE(collision_info)
end

function END()
end
