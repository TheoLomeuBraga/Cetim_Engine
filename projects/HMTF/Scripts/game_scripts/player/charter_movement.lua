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
require("game_scripts.resources.playable_scene")

require("math")

has_jump_booster = 1
local jump_booster_used = false


local this_object = {}
camera = {}
camera_ptr = ""
check_top = {}
check_down = {}

direction_reference = {}
linear_velocity = { x = 0, y = 0, z = 0 }

local core_obj = {}

local layers = {}


local game_states = {
    play = 0,
    conversation = 1,
}

game_state = 0


camera_movement_intensity = 2

this_physics_3d = {}


function START()

    global_data.inventory = {
        jump_boost = 1,
        jump_dash = 1,
    }

    global_data.player_object_ptr = this_object_ptr

    core_obj = game_object(global_data.core_object_ptr)

    camera = create_camera_perspective(this_object_ptr, { x = 0, y = 0.5, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 10000)
    camera_ptr = camera.object_ptr
    global_data.camera_ptr = camera_ptr
    set_lisener_object(camera.object_ptr)

    layers = global_data.layers
    check_top = create_collision_3D(layers.cenary, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(0.75, 0.75, 0.75), true,
        collision_shapes.cylinder, nil, true)
    check_down = create_collision_3D(layers.cenary, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(0.75, 0.75, 0.75),
        true, collision_shapes.cylinder, nil, true)



    this_object = game_object(this_object_ptr)
    this_object.components.transform:change_rotation(0, 0, 0)

    this_physics_3d = this_object.components[components.physics_3D]
    this_physics_3d.boady_dynamic = boady_dynamics.dynamic
    this_physics_3d.rotate_x = false
    this_physics_3d.rotate_y = false
    this_physics_3d.rotate_z = false
    this_physics_3d.friction = 0
    this_physics_3d.gravity_scale = 0
    this_physics_3d.triger = false
    this_physics_3d.collision_shape = collision_shapes.cylinder
    this_physics_3d.scale = Vec3:new(1, 2, 1)
    this_physics_3d:set()


    direction_reference = game_object(this_object_ptr)
    direction_reference.components.transform:set()
end

speed = 16
speed_boost = 1
speed_boost_air = 1
mouse_sensitivity = 0

hit_top = false
hit_down = false

local inputs = {}
local inputs_last_frame = {}

camera_rotation = { x = 180, y = 0 }

this_object_physics_3D_seted = false

jump_power = 20
boost_power = 25
land_friction = 30
--jump_power = 1000

inpulse = { x = 0, y = 0, z = 0 }


base_directional_input = { x = 0, y = 0, z = 0 }
directional_input = { x = 0, y = 0, z = 0 }


platform_movement = { x = 0, y = 0, z = 0 }



pause_last_frame = false

extra_jumps_utilizeds = 0




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



function manage_inpulse_land(i)
    local delta_friction = land_friction * time.delta

    if math.abs(i.x) < delta_friction then
        i.x = 0
    end
    if math.abs(i.z) < delta_friction then
        i.z = 0
    end

    if i.x > 0 then
        i.x = i.x - (delta_friction)
    elseif i.x < 0 then
        i.x = i.x + (delta_friction)
    end

    if i.z > 0 then
        i.z = i.z - (delta_friction)
    elseif i.z < 0 then
        i.z = i.z + (delta_friction)
    end

    if i.x < 0 then
        i.x = i.x + (time.delta * land_friction)
    elseif i.x > 0 then
        i.x = i.x - (time.delta * land_friction)
    end

    if i.z < 0 then
        i.z = i.z + (time.delta * land_friction)
    elseif i.z > 0 then
        i.z = i.z - (time.delta * land_friction)
    end
end

function UPDATE()
    time:get()

    if game_state == game_states.play then
        gravity:get()

        mouse_sensitivity = global_data.mouse_sensitivity

        inputs = global_data.inputs
        inputs_last_frame = global_data.inputs_last_frame

        if inputs.menu > 0 and not (inputs_last_frame.menu > 0) then
            if global_data.pause_menu_obj_ptr == nil then
                menus.open_pause_menu()
            end
        end

        enable_cursor(global_data.pause > 0)

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

        this_object.components.transform:get()
        local pos = deepcopy(this_object.components.transform.position)

        --hit top
        check_top.components.transform:change_position(pos.x, pos.y + 1.75, pos.z)

        hit_top = #check_top.components[components.physics_3D]:get_objects_coliding() > 1

        --hit down
        check_down.components.transform:change_position(pos.x, pos.y - 1.75, pos.z)
        check_down.components[components.physics_3D]:get()
        hit_down = #check_down.components[components.physics_3D]:get_objects_coliding() > 1

        if global_data.pause < 1 and global_data.interacting == 0 then
            --move camera
            window:get()
            camera_rotation.x = camera_rotation.x - (inputs.mouse_view_x) * mouse_sensitivity * 20
            camera_rotation.y = math.max(
                math.min(camera_rotation.y - ((inputs.mouse_view_y) * mouse_sensitivity * 20), 90), -90)

            camera_rotation.x = camera_rotation.x - (inputs.analog_view_x) * mouse_sensitivity
            camera_rotation.y = math.max(
                math.min(camera_rotation.y - ((inputs.analog_view_y) * mouse_sensitivity), 90), -90)

            if not this_object_physics_3D_seted then
                this_physics_3d:set()
                this_object_physics_3D_seted = not this_object_physics_3D_seted
            end



            --get floor info
            local hit = false
            local hit_info = {}
            hit, hit_info = raycast_3D(
                direction_reference.components.transform:get_global_position(0, -1, 0),
                direction_reference.components.transform:get_global_position(0, -10, 0))
            if not hit or not hit_down then
                hit_info.normal = { x = 0, y = 1, z = 0 }
            end

            --get movement direction
            local input_dir = direction_reference.components.transform:get_local_direction(inputs.foward, 0, -inputs
                .left)
            local move_dir = crossProduct(input_dir, hit_info.normal)

            --hit floor
            if hit_down and not (inpulse.y > 0) then
                inpulse.y = 0
                extra_jumps_utilizeds = 0
            end

            --jump
            if hit_down and inpulse.y <= 0 and inputs.jump > 0 and not (inputs_last_frame.jump > 0) then
                inpulse.y = jump_power
                base_directional_input = deepcopy(input_dir)

                this_object.components.audio_source.path = "Audio/sounds/jump.wav"
                this_object.components.audio_source.volume = 20
                this_object.components.audio_source:set()
            end

            directional_input = crossProduct(base_directional_input, hit_info.normal)

            --hit cealing
            if hit_top and inpulse.y > 0 then
                inpulse.y = 0
            end



            local linear_velocity = { x = 0, y = 0, z = 0 }

            --move
            if hit_down and inpulse.y <= 0 then
                --inpulse
                manage_inpulse_land(inpulse)
                
                jump_booster_used = false


                --movement

                linear_velocity = {
                    x = (move_dir.x * (speed + speed_boost)) + platform_movement.x + inpulse.x * time.delta,
                    y = (move_dir.y * (speed + speed_boost)) + platform_movement.y + inpulse.y * time.delta,
                    z = (move_dir.z * (speed + speed_boost)) + platform_movement.z + inpulse.z * time.delta
                }
            else
                linear_velocity = {
                    x = (move_dir.x * (speed + speed_boost_air)) + inpulse.x * time.delta,
                    y = inpulse.y,
                    z = (move_dir.z * (speed + speed_boost_air)) + inpulse.z * time.delta
                }
            end
            

            if has_jump_booster == 1 and jump_booster_used == false and hit_down == false and inputs.jump > 0 and not (inputs_last_frame.jump > 0) then

                inpulse.y = jump_power
                base_directional_input = deepcopy(input_dir)
                
                if math.abs(move_dir.x) + math.abs(move_dir.z) > 0.5 then
                    inpulse.x = boost_power * move_dir.x
                    inpulse.z = boost_power * move_dir.z 
                    print(boost_power * move_dir.x)
                end


                this_object.components.audio_source.path = "Audio/sounds/jump.wav"
                this_object.components.audio_source.volume = 20
                this_object.components.audio_source:set()

                jump_booster_used = true
            end

            this_physics_3d:set_linear_velocity(linear_velocity.x, linear_velocity.y, linear_velocity.z)

            if not hit_down then
                inpulse.y = inpulse.y + (time.delta * time.scale * gravity.force.y * 2)
            end
        else
            this_physics_3d:set_linear_velocity(0, 0, 0)
        end



        camera.components.transform:change_rotation(-camera_rotation.y, 0, inputs.left * camera_movement_intensity)
        this_object.components.transform:change_rotation(0, camera_rotation.x, 0)
        pause_last_frame = global_data.pause < 1


        --this_object.components.transform:get()
        --print("A",this_object.components.transform.position.x,this_object.components.transform.position.y,this_object.components.transform.position.z)

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
    global_data.player_object_ptr = ""
end
