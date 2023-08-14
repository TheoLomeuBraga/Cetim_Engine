require("TMP_libs.objects.global_data")
require("TMP_libs.objects.input")
require("TMP_libs.functions")

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
}



function START()
    inputs_last_frame = deepcopy(inputs)
end

function UPDATE()

    

    deepprint(inputs_last_frame)

    global_data:set_var("inputs_last_frame",inputs_last_frame)
    global_data:set_var("inputs",inputs)
    inputs_last_frame = deepcopy(inputs)
end

function COLLIDE(collision_info)
end

function END()
end
