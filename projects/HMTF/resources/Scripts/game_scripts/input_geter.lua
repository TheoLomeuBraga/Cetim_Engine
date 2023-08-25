require("engine_libs.objects.global_data")
require("engine_libs.objects.input")
require("engine_libs.functions")
require("math")

inputs_last_frame = {}

inputs = {
    foward = 0,
    left = 0,
    jump = false,
    interact = false,
    action_1 = false,
    action_2 = false,
    mouse_view_x = 0,
    mouse_view_y = 0,
    analog_view_x = 0,
    analog_view_y = 0,
    menu = false,
}

function apply_death_zone(input,death_zone)
    local ret = input
    if math.abs(input) < death_zone then
        ret = 0
    end
    return ret
end

function START()
    inputs_last_frame = deepcopy(inputs)
end

function UPDATE()

    local analog_foward =  apply_death_zone(keys_axis:get_input_joystick(1,"ry"),0.2)
    local analog_left =  apply_death_zone(keys_axis:get_input_joystick(1,"rx"),0.2)

    local av_x = apply_death_zone(keys_axis:get_input_joystick(1,"lx"),0.2)
    local av_y = apply_death_zone(keys_axis:get_input_joystick(1,"ly"),0.2)

    inputs = {
        foward = keys_axis:get_input(input_devices.keyboard,"w") - keys_axis:get_input(input_devices.keyboard,"s") - analog_foward,
        left = keys_axis:get_input(input_devices.keyboard,"a") - keys_axis:get_input(input_devices.keyboard,"d") - analog_left,
        jump = keys_axis:get_input(input_devices.keyboard,"space") + keys_axis:get_input_joystick(1,"a"),
        interact = keys_axis:get_input(input_devices.keyboard,"e") + keys_axis:get_input_joystick(1,"y"),
        action_1 = keys_axis:get_input(input_devices.mouse,"left") + keys_axis:get_input_joystick(1,"rt_axis"),
        action_2 = keys_axis:get_input(input_devices.mouse,"right") + keys_axis:get_input_joystick(1,"lt_axis"),
        mouse_view_x = keys_axis:get_input(input_devices.mouse,"movement_x"),
        mouse_view_y = keys_axis:get_input(input_devices.mouse,"movement_y"),
        analog_view_x = av_x,
        analog_view_y = av_y,
        menu = keys_axis:get_input(input_devices.keyboard,"escape") + keys_axis:get_input_joystick(1,"start"),
    }

    if inputs.foward < -1 then
        inputs.foward = -1
    elseif inputs.foward > 1 then
        inputs.foward = 1
    end
    --inputs.foward = -inputs.foward


    if inputs.left < -1 then
        inputs.left = -1
    elseif inputs.left > 1 then
        inputs.left = 1
    end
    --inputs.left = -inputs.left
    

    
    global_data:set("inputs",inputs)
    global_data:set("inputs_last_frame",inputs_last_frame)
    inputs_last_frame = deepcopy(inputs)
end

function COLLIDE(collision_info)
end

function END()
end
