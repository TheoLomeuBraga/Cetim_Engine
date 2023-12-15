require("objects.global_data")
require("objects.input")
require("functions")
require("math")

require("components.extras")
require("components.ui_component")

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

local main_input_method = "keyboard"
function UPDATE()

    local analog_foward =  apply_death_zone(keys_axis:get_input_joystick(1,"ly"),0.2)
    local analog_left =  apply_death_zone(keys_axis:get_input_joystick(1,"lx"),0.2)

    local av_x = apply_death_zone(keys_axis:get_input_joystick(1,"rx"),0.2)
    local av_y = apply_death_zone(keys_axis:get_input_joystick(1,"ry"),0.2)

    inputs = {
        foward = keys_axis:get_input(input_devices.keyboard,"w") - keys_axis:get_input(input_devices.keyboard,"s") - analog_foward,
        left = keys_axis:get_input(input_devices.keyboard,"a") - keys_axis:get_input(input_devices.keyboard,"d") - analog_left,
        jump = keys_axis:get_input(input_devices.keyboard,"space") + keys_axis:get_input_joystick(1,"la"),
        interact = keys_axis:get_input(input_devices.keyboard,"e") + keys_axis:get_input_joystick(1,"y"),
        action_1 = keys_axis:get_input(input_devices.mouse,"left") + (keys_axis:get_input_joystick(1,"rt")) ,
        action_2 = keys_axis:get_input(input_devices.mouse,"right") + (keys_axis:get_input_joystick(1,"lt")) ,
        mouse_pos_x = keys_axis:get_input(input_devices.mouse,"normalized_x"),
        mouse_pos_y = keys_axis:get_input(input_devices.mouse,"normalized_y"),
        mouse_view_x = keys_axis:get_input(input_devices.mouse,"movement_x"),
        mouse_view_y = keys_axis:get_input(input_devices.mouse,"movement_y"),
        analog_view_x = av_x,
        analog_view_y = av_y,
        menu = keys_axis:get_input(input_devices.keyboard,"escape") + keys_axis:get_input_joystick(1,"start"),
    }

    if keys_axis:get_input(input_devices.mouse,"movement_x") + keys_axis:get_input(input_devices.mouse,"movement_y") + keys_axis:get_input(input_devices.mouse,"left") > 0.01 then
        main_input_method = "keyboard"
    elseif analog_foward + analog_left + av_x + av_y > 0 then
        main_input_method = "joystick"
    end

    
    

    inputs.action_1 = math.max(0,inputs.action_1)
    inputs.action_2 = math.max(0,inputs.action_2)

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

    
    if main_input_method == "keyboard" then
        set_ui_curson_location({x=keys_axis:get_input(input_devices.mouse,"normalized_x"),y=keys_axis:get_input(input_devices.mouse,"normalized_y")},inputs.action_1 > 0)
    elseif main_input_method == "joystick" then
    end
    
end

function COLLIDE(collision_info)
end

function END()
end
