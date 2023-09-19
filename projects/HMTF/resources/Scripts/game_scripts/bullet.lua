require("engine_libs.definitions")
require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")

require("engine_libs.objects.game_object")
require("engine_libs.objects.time")

require("engine_libs.objects.material")
require("engine_libs.objects.global_data")
require("engine_libs.objects.vectors")
require("engine_libs.objects.gravity")
require("engine_libs.objects.scene_3D")

require("engine_libs.short_cuts.create_collision")
require("engine_libs.short_cuts.create_mesh")

damage = 1
direction = {x = 1,y = 0,z = 0}
speed = 1
special_pattern = ""
max_distance = -1
life_time = 10
mesh = {
    file = "",
    name = ""
}


function START()
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end
