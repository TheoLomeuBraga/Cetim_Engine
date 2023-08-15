require("engine_libs.definitions")
require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")
require("engine_libs.objects.game_object")
require("engine_libs.objects.time")
require("math")
require("engine_libs.objects.material")
require("engine_libs.objects.global_data")
require("engine_libs.short_cuts.create_camera")
require("engine_libs.objects.vectors")
require("engine_libs.objects.input")
require("engine_libs.objects.gravity")
require("engine_libs.objects.scene_3D")


local this_object = {}
camera = {}

check_top = {}
check_down = {}

hit_top = false
hit_down = false

function START()
    this_object = game_object:new(this_object_ptr)
    camera = create_camera_perspective(this_object_ptr, { x = 0, y = 0.5, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)

end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end