require("definitions")
require("components.extras")
require("components.component_index")
require("components.component_all")
require("objects.game_object")
require("objects.global_data")

local core_obj = nil

function summon_entity(pos,rot_y,type)
    
    if core_obj == nil then
        core_obj = game_object(global_data.core_object_ptr)
    end
    
    local args = {pos=pos,rot_y=rot_y,type=type}
    core_obj.components.lua_scripts.scripts["game_scripts/entity_master"].functions.summon_entity(args) 

end