
require("definitions")
require("components.extras")
require("components.component_all")
require("components.component_index")

require("objects.game_object")
require("objects.time")

require("objects.material")
require("objects.global_data")
require("objects.vectors")
require("objects.gravity")
require("objects.scene_3D")

require("short_cuts.create_collision")
require("short_cuts.create_mesh")

require("objects.global_data")

local bullets_list = {}
local layers = {}

bullet_data = {}
function bullet_data:new()
    
end

function add_bullet(data)
    
end

function remove_bullet(adres)
    
end

function START()
    layers = global_data:get_var("layers")
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end

function summon_bullet(args)
    print("bullet")
    
    return {}
end


