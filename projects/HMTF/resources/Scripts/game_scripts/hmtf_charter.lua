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
require("engine_libs.objects.window")

require("engine_libs.short_cuts.create_camera")
require("engine_libs.short_cuts.create_collision")
require("resources.playable_scene")

require("math")


local this_object = {}
camera = {}
check_top = {}
check_down = {}

local layers = {}



function START()
    this_object = game_object:new(this_object_ptr)
    camera = create_camera_perspective(this_object_ptr, { x = 0, y = 0.5, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)

    layers = global_data:get_var("layers")
    check_top = create_collision_3D(layers.cenary, Vec3:new(0,0,0), Vec3:new(0,0,0), Vec3:new(1,0.5,1), true,collision_shapes.cylinder,nil,true)
    check_down = create_collision_3D(layers.cenary, Vec3:new(0,0,0), Vec3:new(0,0,0), Vec3:new(1,0.5,1), true,collision_shapes.cylinder,nil,true)

    
    this_object:add_component(components.physics_3D)
    this_object.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
    this_object.components[components.physics_3D].rotate_X = false
    this_object.components[components.physics_3D].rotate_Y = false
    this_object.components[components.physics_3D].rotate_Z = false
    this_object.components[components.physics_3D].friction = 0
    this_object.components[components.physics_3D].collision_shape = collision_shapes.capsule
    this_object.components[components.physics_3D].scale = Vec3:new(1,2,1)
    this_object.components[components.physics_3D]:set()
    
end

speed = 7
mouse_sensitivity = 0

hit_top = false
hit_down = false

local inputs = {}
local inputs_last_frame = {}

function UPDATE()

    mouse_sensitivity = global_data:get_var("mouse_sensitivity")

    this_object.components[components.transform]:get()
    local pos = deepcopy(this_object.components[components.transform].position)

    
    check_top.components[components.transform]:change_position(pos.x,pos.y + 1.5 ,pos.z)
    check_top.components[components.physics_3D]:get()
    hit_top = tablelength(check_top.components[components.physics_3D].objs_touching) > 1
    
    check_down.components[components.transform]:change_position(pos.x,pos.y - 1.5 ,pos.z)
    check_down.components[components.physics_3D]:get()
    hit_down = tablelength(check_down.components[components.physics_3D].objs_touching) > 1

    inputs = global_data:get("inputs")
    inputs_last_frame = global_data:get("inputs_last_frame")

    if inputs.menu > 0 and not (inputs_last_frame.menu > 0) then
        if menu.obj == nil then
            menu.open()
        else
            menu.close()
        end
    end

    if global_data:get("pause") < 1 then
        window:get()
        keys_axis:set_cursor_position(window.resolution.x / 2, window.resolution.y / 2)
    end

end

function COLLIDE(collision_info)
end

function END()
    remove_object(camera.object_ptr)
    remove_object(check_top.object_ptr)
    remove_object(check_down.object_ptr)
end