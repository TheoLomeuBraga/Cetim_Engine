require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.game_object")
require("math")

target_name = ""

no_textures_y = 0.0

parts_ptr_list = {}

local target = nil

this_object = nil

function get_texture_pos_y()

    if this_object == nil then
        this_object = game_object(this_object_ptr)
    end

    if this_object.components.transform ~= nil then
        this_object.components.transform:get()
        local ret = math.floor(this_object.components.transform.position.y + 0.1) / no_textures_y
        return ret
    end

    return 0
    
end

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

    

    target.components.render_mesh:get()
    target.components.render_mesh.materials[1].position_scale.y = get_texture_pos_y()
    target.components.render_mesh:set()

end

function COLLIDE(collision_info)
end

function END()
end
