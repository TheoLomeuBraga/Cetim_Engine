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

health = 100
max_health = 100
inventory = {
    extra_jumps = 1,
    jump_booster = 5,
    gun = 0,
    sword = 0,
    super_charger = 0,
}

game_states = {
    play = 0,
    conversation = 1,
}

game_state = 0

local cannon = {
    obj = {},
    part_list = {},
    animations = {},
}

function create_arm_cannon()
    local sceane_3D_data = get_scene_3D("resources/3D Models/arm_cannon.gltf")
    local entity_data = cenary_builders.entity(camera.object_ptr,4, sceane_3D_data,false,false)
    cannon.obj = deepcopy(entity_data.obj)
    cannon.part_list = deepcopy(entity_data.parts_list)
end

local animation_state = {
    animation = {},
    loop = false,
}

function stop_interact_arm_cannon_animation(animation,loop)

    animation_state.animation = deepcopy(animation)
    animation_state.loop = loop
    
end

function play_arm_cannon_animation()
    
end

function START()

    global_data:set("player_object_ptr",this_object_ptr)

    this_object = game_object:new(this_object_ptr)
    camera = create_camera_perspective(this_object_ptr, { x = 0, y = 0.5, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    set_lisener_object(camera.object_ptr)

    layers = global_data:get_var("layers")
    check_top = create_collision_3D(layers.cenary, Vec3:new(0,0,0), Vec3:new(0,0,0), Vec3:new(0.75,0.75,0.75), true,collision_shapes.cylinder,nil,true)
    check_down = create_collision_3D(layers.cenary, Vec3:new(0,0,0), Vec3:new(0,0,0), Vec3:new(0.75,0.75,0.75), true,collision_shapes.cylinder,nil,true)

    this_object.components[components.transform]:change_rotation(0,0,0)
    
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

    this_object:add_component(components.audio_source)

    create_arm_cannon()
    
end

speed = 12
speed_boost = 1
speed_boost_air = 1
mouse_sensitivity = 0

hit_top = false
hit_down = false

local inputs = {}
local inputs_last_frame = {}

camera_rotation = {x=180,y=0}

this_object_physics_3D_seted = false

force_y = 12

inpulse_y = 0

base_directional_inpulse = {x=0,y=0,z=0}
directional_inpulse = {x=0,y=0,z=0}


platform_movement = {x=0,y=0,z=0}

friction = 10
air_friction = 0.5

pause_last_frame = false

extra_jumps_utilizeds = 0

function aproche_to_zero(num,speed)
    ret = 0
    if math.abs(num) < speed then
        ret = 0
    elseif num < 0 then
        ret = num + speed
    elseif num > 0 then
        ret = num - speed
    end
    return ret
end

function aproche_to_target_value(num,speed,target_value)
    ret = target_value
    if math.abs(num) < speed then
        ret = target_value
    elseif num < target_value then
        ret = num + speed
    elseif num > target_value then
        ret = num - speed
    end
    return ret
end

function interact()

    local ray_start = camera.components[components.transform]:get_global_position(0,0,0)
    local ray_end_direction = camera.components[components.transform]:get_local_direction(0,0,-10)
    local ray_end = {x=ray_start.x - ray_end_direction.x,y=ray_start.y - ray_end_direction.y,z=ray_start.z - ray_end_direction.z}
    
    local hit = false
    local hit_info = {}
    hit,hit_info = raycast_3D(ray_start,ray_end)

    if hit then

        local hit_object = game_object:new(hit_info.collision_object)

        if  hit_object.components ~= nil and hit_object.components[components.lua_scripts] ~= nil and hit_object.components[components.lua_scripts]:has_script("game_scripts/mensage") and inputs.interact > 0 and inputs_last_frame.interact < 1 then
            hit_object.components[components.lua_scripts]:call_function("game_scripts/mensage","interact",{})
        end

    end

end

function UPDATE()

    if game_state == game_states.play then
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

        enable_cursor(global_data:get("pause") > 0)
    
        this_object.components[components.transform]:get()
        local pos = deepcopy(this_object.components[components.transform].position)

        --hit top
        check_top.components[components.transform]:change_position(pos.x,pos.y + 1.75 ,pos.z)
        check_top.components[components.physics_3D]:get()
        hit_top = tablelength(check_top.components[components.physics_3D].objs_touching) > 1
    
        --hit down
        check_down.components[components.transform]:change_position(pos.x,pos.y - 1.75 ,pos.z)
        check_down.components[components.physics_3D]:get()
        hit_down = tablelength(check_down.components[components.physics_3D].objs_touching) > 1

        if global_data:get("pause") < 1 then
            
            --interact
            if inputs.interact > 0 and inputs_last_frame.interact < 1 then
                interact()
            end
        
            

            --move camera
            window:get()
            camera_rotation.x = camera_rotation.x-(inputs.mouse_view_x) * mouse_sensitivity * 20
            camera_rotation.y = math.max(math.min(camera_rotation.y-((inputs.mouse_view_y) * mouse_sensitivity * 20),90),-90)

            camera_rotation.x = camera_rotation.x-(inputs.analog_view_x) * mouse_sensitivity / 2.5
            camera_rotation.y = math.max(math.min(camera_rotation.y-((inputs.analog_view_y) * mouse_sensitivity / 2.5 ),90),-90)

            if not this_object_physics_3D_seted then
                this_object.components[components.physics_3D]:set()
                this_object_physics_3D_seted = not this_object_physics_3D_seted
            end
        
        
        
            --get floor info
            local hit = false
            local hit_info = {}
            hit,hit_info = raycast_3D(direction_reference.components[components.transform]:get_global_position(0,-1,0),direction_reference.components[components.transform]:get_global_position(0,-10,0))
            if not hit or not hit_down then
                hit_info.normal = {x=0,y=1,z=0}
            end

            --get movement direction
            local input_dir = direction_reference.components[components.transform]:get_local_direction(inputs.foward ,0,-inputs.left )
            local move_dir = crossProduct(input_dir,hit_info.normal)

            --hit floor
            if hit_down and not (inpulse_y > 0)  then
                inpulse_y = 0
                extra_jumps_utilizeds = 0
            end

            --jump
            if hit_down and inpulse_y <= 0 and inputs.jump > 0 and not (inputs_last_frame.jump > 0) then
                inpulse_y = force_y
                base_directional_inpulse = deepcopy(input_dir)

                this_object.components[components.audio_source].path = "resources/Audio/sounds/jump.wav"
                this_object.components[components.audio_source].volume = 20
                this_object.components[components.audio_source]:set()
            end

            directional_inpulse  = crossProduct(base_directional_inpulse,hit_info.normal)

            --hit cealing
            if hit_top and inpulse_y > 0 then
                inpulse_y = 0
            end

           --move
            if hit_down and not (inpulse_y > 0) then
                this_object.components[components.physics_3D]:set_linear_velocity((move_dir.x * (speed + speed_boost)) + platform_movement.x * time.sacale,(move_dir.y * (speed + speed_boost)) + platform_movement.y * time.sacale,(move_dir.z * (speed + speed_boost)) + platform_movement.z  * time.sacale)
            else
                this_object.components[components.physics_3D]:set_linear_velocity((move_dir.x * (speed + speed_boost_air)) * time.sacale,(move_dir.y * (speed + speed_boost_air)) + inpulse_y * time.sacale,(move_dir.z * (speed + speed_boost_air)) * time.sacale) 
            end

            if not hit_down then
                inpulse_y = inpulse_y + ( time.delta * gravity.force.y )
            end
        
        else

            
            if not hit_down then
                inpulse_y = inpulse_y + gravity.force.y
                this_object.components[components.physics_3D]:set_linear_velocity(0,inpulse_y * time.sacale,0)
            else
                this_object.components[components.physics_3D]:set_linear_velocity(0,0,0)
            end
            
            
        end

        camera.components[components.transform]:change_rotation(-camera_rotation.y,0,0)
        this_object.components[components.transform]:change_rotation(0,camera_rotation.x,0)
        pause_last_frame = global_data:get("pause") < 1
    end

    play_arm_cannon_animation()

end

function COLLIDE(collision_info)
end

function END()
    remove_object(camera.object_ptr)
    remove_object(check_top.object_ptr)
    remove_object(check_down.object_ptr)
    global_data:set("player_object_ptr","")
end