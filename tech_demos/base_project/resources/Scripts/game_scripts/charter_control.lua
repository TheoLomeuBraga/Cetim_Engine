require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.game_object")
require("TMP_libs.objects.input")
require("TMP_libs.objects.time")
require("TMP_libs.objects.global_data")
require("TMP_libs.objects.window")
require("math")

layers = {}
charter_type = "2D"
charter_size = {x=1,y=1,z=1}

local this_object = {}

local detect_top = {}
local detect_down = {}





function START()
    this_object = game_object:new(this_object_ptr)

    detect_top = game_object:new(create_object(layers.cenary))
    detect_top:add_component(components.transform)
    detect_top.components[components.transform]:set()

    detect_down = game_object:new(create_object(layers.cenary))
    detect_down:add_component(components.transform)
    detect_down.components[components.transform]:set()

    
    if charter_type == "2D" then

    elseif charter_type == "3D" then

    end

end

function UPDATE()

    if charter_type == "2D" then

    elseif charter_type == "3D" then

    end

end

function END()
    remove_object(detect_top.object_ptr)
    detect_top = nil 

    remove_object(detect_down.object_ptr)
    detect_down = nil

    if charter_type == "2D" then

    elseif charter_type == "3D" then

    end

end

function COLLIDE(collision_info)

    if charter_type == "2D" then

    elseif charter_type == "3D" then

    end

end