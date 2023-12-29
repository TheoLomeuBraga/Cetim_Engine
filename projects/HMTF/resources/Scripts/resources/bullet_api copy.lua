require("definitions")
require("components.extras")
require("components.component_index")
require("components.component_all")
require("objects.game_object")
require("objects.global_data")

local core_obj = nil

function summon_enemy(pos,rot_y,type)
    
    if core_obj == nil then
        core_obj = game_object(global_data.core_object_ptr)
    end
    
end