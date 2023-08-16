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

direction_reference = {}

local layers = {}



function START()

    global_data:set("player_object_ptr",this_object_ptr)

    this_object = game_object:new(this_object_ptr)
    camera = create_camera_perspective(this_object_ptr, { x = 0, y = 0.5, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)

    layers = global_data:get_var("layers")
    check_top = create_collision_3D(layers.cenary, Vec3:new(0,0,0), Vec3:new(0,0,0), Vec3:new(0.5,0.5,0.5), true,collision_shapes.cylinder,nil,true)
    check_down = create_collision_3D(layers.cenary, Vec3:new(0,0,0), Vec3:new(0,0,0), Vec3:new(0.5,0.5,0.5), true,collision_shapes.cylinder,nil,true)

    this_object.components[components.transform]:change_rotation(0,180,0)
    
    this_object:add_component(components.physics_3D)
    this_object.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
    this_object.components[components.physics_3D].rotate_X = false
    this_object.components[components.physics_3D].rotate_Y = false
    this_object.components[components.physics_3D].rotate_Z = false
    this_object.components[components.physics_3D].friction = 0
    this_object.components[components.physics_3D].gravity_scale = 0
    this_object.components[components.physics_3D].triger = false
    this_object.components[components.physics_3D].collision_shape = collision_shapes.capsule
    this_object.components[components.physics_3D].scale = Vec3:new(1,2,1)
    this_object.components[components.physics_3D]:set()

    direction_reference = game_object:new(this_object_ptr)
    direction_reference:add_component(components.transform)
    direction_reference.components[components.transform]:set()

    
end

speed = 12
mouse_sensitivity = 0

hit_top = false
hit_down = false

local inputs = {}
local inputs_last_frame = {}

camera_rotation = {x=180,y=0}

this_object_physics_3D_seted = false

force_y = 12

inpulse = {x=0,y=0,z=0}

inventory = {
    gun = 0,
    sword = 0,
    super_charger = 0,
}

function UPDATE()

    time:get()
    gravity:get()

    mouse_sensitivity = global_data:get("mouse_sensitivity")

    

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
        this_object.components[components.transform]:get()
        local pos = deepcopy(this_object.components[components.transform].position)

    
        check_top.components[components.transform]:change_position(pos.x,pos.y + 1.5 ,pos.z)
        check_top.components[components.physics_3D]:get()
        hit_top = tablelength(check_top.components[components.physics_3D].objs_touching) > 1
    
    
        check_down.components[components.transform]:change_position(pos.x,pos.y - 1.5 ,pos.z)
        check_down.components[components.physics_3D]:get()
        hit_down = tablelength(check_down.components[components.physics_3D].objs_touching) > 1

        window:get()
        keys_axis:set_cursor_position(window.resolution.x / 2, window.resolution.y / 2)

        camera_rotation.x = -((inputs.mouse_view_x - 0.5) * mouse_sensitivity * 20)
        camera_rotation.y = math.max(math.min(camera_rotation.y - ((inputs.mouse_view_y - 0.5) * mouse_sensitivity * 20),90),-90)
        camera.components[components.transform]:change_rotation(camera_rotation.y,0,0)
        this_object.components[components.physics_3D]:set_angular_velocity(0,camera_rotation.x,0)

        if not this_object_physics_3D_seted then
            this_object.components[components.physics_3D]:set()
            this_object_physics_3D_seted = not this_object_physics_3D_seted
        end
        
        if hit_down and not (inpulse.y > 0)  then
            inpulse.y = 0
        end
        if hit_down and inpulse.y <= 0 and inputs.jump > 0 and not (inputs_last_frame.jump > 0) then
            inpulse.y = force_y
        end
        if hit_top and inpulse.y > 0 then
            inpulse.y = 0
        end
        
        local hit = false
        local hit_info = {}
        hit,hit_info = raycast_3D(direction_reference.components[components.transform]:get_global_position(0,-1,0),direction_reference.components[components.transform]:get_global_position(0,-10,0))
        if not hit or not hit_down then
            hit_info.normal = {x=0,y=1,z=0}
        end

        local move_dir = direction_reference.components[components.transform]:get_local_direction(inputs.foward ,0,-inputs.left )

        move_dir = crossProduct(move_dir,hit_info.normal)

        this_object.components[components.physics_3D]:set_linear_velocity(move_dir.x * time.sacale * speed,(move_dir.y * time.sacale * speed) + inpulse.y,move_dir.z * time.sacale * speed)

        if not hit_down then
            inpulse.y = inpulse.y + (time.delta * gravity.force.y )
        end
        

    end

end

function COLLIDE(collision_info)
end

function END()
    remove_object(camera.object_ptr)
    remove_object(check_top.object_ptr)
    remove_object(check_down.object_ptr)
    global_data:set("player_object_ptr","")
end