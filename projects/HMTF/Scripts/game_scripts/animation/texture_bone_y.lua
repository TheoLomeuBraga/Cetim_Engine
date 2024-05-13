require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.game_object")


target_name = ""

no_textures_y = 0

parts_ptr_list = {}

local target = nil

function START()
end

function UPDATE()
    
    if target == nil then
        for key, value in pairs(parts_ptr_list) do
            local obj = game_object(value)
            obj:get()
            if obj.name == target_name then
                target = game_object(value)
                break
            end
        end
    end

end

function COLLIDE(collision_info)
end

function END()
end
