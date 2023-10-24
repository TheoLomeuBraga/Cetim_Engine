require("objects.game_object")
require("components.component_all")
require("objects.time")

speed = 500

obj_ptr_to_rotate = nil
local obj_to_rotate = nil

function START()
    if obj_ptr_to_rotate ~= nil then
        obj_to_rotate = game_object:new(obj_ptr_to_rotate)
    end
end

local sw = stopwatch:new()

function UPDATE()
    time:get()


    
    if obj_to_rotate ~= nil then
        obj_to_rotate.components[components.transform]:get()
        obj_to_rotate.components[components.transform].rotation.z = obj_to_rotate.components[components.transform].rotation.z + (speed * sw:getTime())
        obj_to_rotate.components[components.transform]:set()

        sw = stopwatch:new()
    end
end

function COLLIDE(collision_info)
end

function END()
end
