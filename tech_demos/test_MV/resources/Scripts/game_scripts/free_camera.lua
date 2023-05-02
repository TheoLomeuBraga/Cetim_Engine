require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.short_cuts.create_camera")

require("TMP_libs.objects.input")

this_object = {}

sensivity = 7
speed = 7


function START()
    this_object = game_object:new(this_object_ptr)
end

function UPDATE()
    if keys_axis:get_input(input_devices.keyboard,input_keys.keyboard[input_keys.keyboard.w]) == 1 then
        print("W")
    end
    if keys_axis:get_input(input_devices.keyboard,input_keys.keyboard[input_keys.keyboard.a]) == 1 then
        print("A")
    end
    if keys_axis:get_input(input_devices.keyboard,input_keys.keyboard[input_keys.keyboard.s]) == 1 then
        print("S")
    end
    if keys_axis:get_input(input_devices.keyboard,input_keys.keyboard[input_keys.keyboard.d]) == 1 then
        print("D")
    end

    deepprint(this_object.components[components.transform]:get_local_direction(1,0,0))
    
end

function COLLIDE(collision_info)
end

function END()
end
