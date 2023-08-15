require("engine_libs.definitions")
require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")

require("engine_libs.objects.game_object")
require("engine_libs.objects.time")

require("engine_libs.objects.material")
require("engine_libs.objects.global_data")
require("engine_libs.objects.vectors")
require("engine_libs.objects.input")
require("engine_libs.objects.gravity")
require("engine_libs.objects.scene_3D")

require("engine_libs.short_cuts.create_camera")
require("engine_libs.short_cuts.create_collision")

require("math")


local this_object = {}
camera = {}
check_top = {}
check_down = {}

local layers = {}

hit_top = false
hit_down = false

function START()
    this_object = game_object:new(this_object_ptr)
    camera = create_camera_perspective(this_object_ptr, { x = 0, y = 0.5, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)

    layers = global_data:get_var("layers")
    check_top = create_collision_3D(layers.cenary, Vec3:new(0,0,0), Vec3:new(0,0,0), Vec3:new(1,1,1), true,collision_shapes.cylinder,nil,true)
    hit_down = create_collision_3D(layers.cenary, Vec3:new(0,0,0), Vec3:new(0,0,0), Vec3:new(1,1,1), true,collision_shapes.cylinder,nil,true)

    
    this_object:add_component(components.physics_3D)
    this_object.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
    this_object.components[components.physics_3D].rotate_X = false
    this_object.components[components.physics_3D].rotate_Y = false
    this_object.components[components.physics_3D].rotate_Z = false
    this_object.components[components.physics_3D].collision_shape = collision_shapes.capsule
    this_object.components[components.physics_3D]:set()
    
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end