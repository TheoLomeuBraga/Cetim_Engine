require("definitions")
require("components.extras")
require("components.component_all")
require("components.component_index")

require("objects.game_object")
require("objects.time")

require("objects.material")
require("objects.global_data")
require("objects.vectors")
require("objects.input")
require("objects.gravity")
require("objects.scene_3D")
require("objects.window")

require("short_cuts.create_camera")
require("short_cuts.create_collision")
require("resources.playable_scene")

require("math")

local this_object = {}
camera = {}
camera_ptr = ""
check_top = {}
check_down = {}

direction_reference = {}
movement_inpulse = {x=0,y=0,z=0}

local core_obj = {}

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

local game_states = {
    play = 0,
    conversation = 1,
}

game_state = 0




this_physics_3d = {}


function START()
    global_data:set("player_object_ptr", this_object_ptr)

    core_obj = game_object(global_data:get("core_object_ptr"))

    camera = create_camera_perspective(this_object_ptr, { x = 0, y = 0.5, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    camera_ptr = camera.object_ptr
    set_lisener_object(camera.object_ptr)

    layers = global_data:get_var("layers")
    check_top = create_collision_3D(layers.cenary, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(0.75, 0.75, 0.75), true,
        collision_shapes.cylinder, nil, true)
    check_down = create_collision_3D(layers.cenary, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(0.75, 0.75, 0.75),
        true, collision_shapes.cylinder, nil, true)


    
    this_object = game_object(this_object_ptr)
    this_object.components[components.transform]:change_rotation(0, 0, 0)

    this_physics_3d = this_object.components[components.physics_3D]
    this_physics_3d.boady_dynamic = boady_dynamics.kinematic
    this_physics_3d.rotate_x = false
    this_physics_3d.rotate_y = false
    this_physics_3d.rotate_z = false
    this_physics_3d.friction = 0
    this_physics_3d.gravity_scale = 0
    this_physics_3d.triger = false
    this_physics_3d.collision_shape = collision_shapes.capsule
    this_physics_3d.scale = Vec3:new(1, 2, 1)
    this_physics_3d:set()


    direction_reference = game_object(this_object_ptr)
    direction_reference.components[components.transform]:set()
    --[[]]
end

speed = 12
speed_boost = 1
speed_boost_air = 1
mouse_sensitivity = 0

hit_top = false
hit_down = false

local inputs = {}
local inputs_last_frame = {}

camera_rotation = { x = 180, y = 0 }

this_object_physics_3D_seted = false

force_y = 24

inpulse_y = 0

base_directional_inpulse = { x = 0, y = 0, z = 0 }
directional_inpulse = { x = 0, y = 0, z = 0 }


platform_movement = { x = 0, y = 0, z = 0 }

friction = 10
air_friction = 0.5

pause_last_frame = false

extra_jumps_utilizeds = 0

local inpulse = {}




function aproche_to_zero(num, speed)
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

function aproche_to_target_value(num, speed, target_value)
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

--[[
function interact()
    local ray_start = camera.components[components.transform]:get_global_position(0, 0, 0)
    local ray_end_direction = camera.components[components.transform]:get_local_direction(0, 0, -10)
    local ray_end = {
        x = ray_start.x - ray_end_direction.x,
        y = ray_start.y - ray_end_direction.y,
        z = ray_start.z - ray_end_direction.z
    }

    local hit = false
    local hit_info = {}
    hit, hit_info = raycast_3D(ray_start, ray_end)

    if hit then
        local hit_object = game_object(hit_info.collision_object)

        if hit_object.components ~= nil and hit_object.components[components.lua_scripts] ~= nil and hit_object.components[components.lua_scripts]:has_script("game_scripts/mensage") and inputs.interact > 0 and inputs_last_frame.interact < 1 then
            hit_object.components[components.lua_scripts]:call_function("game_scripts/mensage", "interact", {})
        end
    end
end
]]



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
        
        this_physics_3d:get()

        local get_valid_touches_top = function(objs_touching)
            local valid_touches = 0
            for key, value in pairs(objs_touching) do
                local obj_touching = game_object(value)
                obj_touching.components[components.physics_3D]:get()
                if not obj_touching.components[components.physics_3D].triger and obj_touching.components[components.physics_3D].boady_dynamic == boady_dynamics.static then
                    valid_touches = valid_touches + 1
                end
            end
            return valid_touches
        end

        

        local get_valid_touches_down = function(objs_touching)
            local valid_touches = 0
            for key, value in pairs(objs_touching) do
                local obj_touching = game_object(value)
                obj_touching.components[components.physics_3D]:get()
                if not obj_touching.components[components.physics_3D].triger then
                    valid_touches = valid_touches + 1
                end
            end
            return valid_touches
        end

        this_object.components[components.transform]:get()
        local pos = deepcopy(this_object.components[components.transform].position)

        --hit top
        check_top.components[components.transform]:change_position(pos.x, pos.y + 1.75, pos.z)
        
        hit_top = get_valid_touches_top(check_top.components[components.physics_3D]:get_objects_coliding()) > 0

        --hit down
        check_down.components[components.transform]:change_position(pos.x, pos.y - 1.75, pos.z)
        check_down.components[components.physics_3D]:get()
        hit_down = get_valid_touches_down(check_down.components[components.physics_3D]:get_objects_coliding()) > 1

        

        if global_data:get("pause") < 1 and global_data:get("interacting") == 0 then

            --move camera
            window:get()
            camera_rotation.x = camera_rotation.x - (inputs.mouse_view_x) * mouse_sensitivity * 20
            camera_rotation.y = math.max(math.min(camera_rotation.y - ((inputs.mouse_view_y) * mouse_sensitivity * 20),90), -90)

            camera_rotation.x = camera_rotation.x - (inputs.analog_view_x) * mouse_sensitivity / 2.5
            camera_rotation.y = math.max(
                math.min(camera_rotation.y - ((inputs.analog_view_y) * mouse_sensitivity / 2.5), 90), -90)

            if not this_object_physics_3D_seted then
                this_physics_3d:set()
                this_object_physics_3D_seted = not this_object_physics_3D_seted
            end

            

            --get floor info
            local hit = false
            local hit_info = {}
            hit, hit_info = raycast_3D(
                direction_reference.components[components.transform]:get_global_position(0, -1, 0),
                direction_reference.components[components.transform]:get_global_position(0, -10, 0))
            if not hit or not hit_down then
                hit_info.normal = { x = 0, y = 1, z = 0 }
            end

            --get movement direction
            local input_dir = direction_reference.components[components.transform]:get_local_direction(inputs.foward, 0,-inputs.left)
            local move_dir = crossProduct(input_dir, hit_info.normal)

            --hit floor
            if hit_down and not (inpulse_y > 0) then
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

            directional_inpulse = crossProduct(base_directional_inpulse, hit_info.normal)

            --hit cealing
            if hit_top and inpulse_y > 0 then
                inpulse_y = 0
            end
            

            local impulse = {x=0,y=0,z=0}
            
            --move
            if hit_down and not (inpulse_y > 0) then
                impulse = { x = (move_dir.x * (speed + speed_boost)) + platform_movement.x,
                    y = (move_dir.y * (speed + speed_boost)) + platform_movement.y,
                    z = (move_dir.z * (speed + speed_boost)) + platform_movement.z }
            else
                impulse = { x = (move_dir.x * (speed + speed_boost_air)),
                    y = (move_dir.y * (speed + speed_boost_air)) + inpulse_y,
                    z = (move_dir.z * (speed + speed_boost_air)) }
            end

            if inputs.action_2 > 0 then
                local turbo_time_speed = 10
                this_physics_3d:set_linear_velocity(impulse.x * turbo_time_speed, impulse.y * turbo_time_speed, impulse.z * turbo_time_speed)
                time:set_speed(1 / turbo_time_speed)
            else 
                time:set_speed(1)
                this_physics_3d:set_linear_velocity(impulse.x, impulse.y, impulse.z)
            end
            

            
            
            movement_inpulse = deepcopy(impulse)

            
            if not hit_down then
                inpulse_y = inpulse_y + (time.delta * gravity.force.y * 2)
            end

        else
            this_physics_3d:set_linear_velocity(0, 0, 0)
        end

        camera.components[components.transform]:change_rotation(-camera_rotation.y, 0, 0)
        this_object.components[components.transform]:change_rotation(0, camera_rotation.x, 0)
        pause_last_frame = global_data:get("pause") < 1

        this_physics_3d:get()
    end

    

    
    --memory_usage_info()
end

function COLLIDE(collision_info)
end

function END()
    remove_object(camera.object_ptr)
    remove_object(check_top.object_ptr)
    remove_object(check_down.object_ptr)
    global_data:set("player_object_ptr", "")
end
