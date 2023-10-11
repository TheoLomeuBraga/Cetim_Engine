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
check_top = {}
check_down = {}

direction_reference = {}

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

game_states = {
    play = 0,
    conversation = 1,
}

game_state = 0

local cannon = {
    obj = {},
    part_list = {},
    part_ptr_list = {},
    animations = {},
}

arm_cannon_sceane_data = {}

function create_arm_cannon()
    arm_cannon_sceane_data = get_scene_3D("resources/3D Models/arm_cannon.gltf")
    local entity_data = cenary_builders.entity(camera.object_ptr, 4, arm_cannon_sceane_data,
        "resources/Shaders/explosive_vertex_mesh", false, false)
    --"resources/Shaders/explosive_vertex_mesh"
    cannon.obj = deepcopy(entity_data.obj)
    cannon.part_list = deepcopy(entity_data.parts_list)
    for key, value in pairs(cannon.part_list) do
        cannon.part_ptr_list[key] = value.object_ptr
    end
end

function set_cannon_vertex_explosion_state(primitives_size, elevation_distance, texture_transition, transition_texture)
    for key_1, value_1 in pairs(cannon.part_list) do
        if value_1:have_component(components.render_mesh) then
            for key_2, value_2 in pairs(value_1.components[components.render_mesh].materials) do
                cannon.part_list[key_1].components[components.render_mesh].materials[key_2].inputs["primitives_size"] =
                    primitives_size
                cannon.part_list[key_1].components[components.render_mesh].materials[key_2].inputs["elevation_distance"] =
                    elevation_distance
                cannon.part_list[key_1].components[components.render_mesh].materials[key_2].inputs["texture_transition"] =
                    texture_transition
                cannon.part_list[key_1].components[components.render_mesh].materials[key_2].textures[2] =
                    transition_texture
                cannon.part_list[key_1].components[components.render_mesh]:set()
            end
        end
    end
end

local animation_state = {
    name = "",
    animation = nil,
    loop = false,
    speed = 1,
    time = 0,
    finish = false,
}

function start_arm_cannon_animation(animation_name, speed, loop)
    if animation_state.name ~= animation_name then
        animation_state.name = animation_name
        animation_state.animation = deepcopy(arm_cannon_sceane_data.animations[animation_name])
        animation_state.speed = speed
        animation_state.loop = loop
        animation_state.time = 0
        animation_state.finish = false
    end
end

function restart_arm_cannon_animation()
    animation_state.time = 0
    animation_state.finish = false
end

function play_arm_cannon_animation()
    if animation_state.animation ~= nil and global_data:get("pause") < 1 then
        time:get()
        animation_state.time = animation_state.time + (animation_state.speed * time.delta)

        if animation_state.time > animation_state.animation.duration then
            if animation_state.loop then
                animation_state.time = 0
            else
                animation_state.time = animation_state.animation.duration
                animation_state.finish = true
            end
        end

        set_keyframe("resources/3D Models/arm_cannon.gltf", cannon.part_ptr_list, false, animation_state.name,animation_state.time)
    end
end

function START()
    global_data:set("player_object_ptr", this_object_ptr)

    core_obj = game_object:new(global_data:get("core_object_ptr"))

    camera = create_camera_perspective(this_object_ptr, { x = 0, y = 0.5, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    camera:add_component(components.audio_source)
    set_lisener_object(camera.object_ptr)

    layers = global_data:get_var("layers")
    check_top = create_collision_3D(layers.cenary, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(0.75, 0.75, 0.75), true,
        collision_shapes.cylinder, nil, true)
    check_down = create_collision_3D(layers.cenary, Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(0.75, 0.75, 0.75),
        true, collision_shapes.cylinder, nil, true)



    this_object = game_object:new(this_object_ptr)
    this_object.components[components.transform]:change_rotation(0, 0, 0)

    this_object:add_component(components.physics_3D)
    this_object.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
    this_object.components[components.physics_3D].rotate_X = false
    this_object.components[components.physics_3D].rotate_Y = false
    this_object.components[components.physics_3D].rotate_Z = false
    this_object.components[components.physics_3D].friction = 0
    this_object.components[components.physics_3D].gravity_scale = 0
    this_object.components[components.physics_3D].triger = false
    this_object.components[components.physics_3D].collision_shape = collision_shapes.capsule
    this_object.components[components.physics_3D].scale = Vec3:new(1, 2, 1)
    this_object.components[components.physics_3D]:set()


    direction_reference = game_object:new(this_object_ptr)
    direction_reference:add_component(components.transform)
    direction_reference.components[components.transform]:set()

    this_object:add_component(components.audio_source)

    create_arm_cannon()

    --start_arm_cannon_animation("normal",1,false)
    --start_arm_cannon_animation("open",1,false)
    --start_arm_cannon_animation("walk",1,true)
    --start_arm_cannon_animation("spin",1,true)
    --start_arm_cannon_animation("recoil",8,true)
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

force_y = 12

inpulse_y = 0

base_directional_inpulse = { x = 0, y = 0, z = 0 }
directional_inpulse = { x = 0, y = 0, z = 0 }


platform_movement = { x = 0, y = 0, z = 0 }

friction = 10
air_friction = 0.5

pause_last_frame = false

extra_jumps_utilizeds = 0

local shoot_data = {}

local impulse = {}

require("resources.bullet_api")

function advanced_shoot(mesh, sound, spred, speed, life_time, damage, quantity,color, hit_scan)
    start_arm_cannon_animation("recoil", 2, false)
    camera.components[components.audio_source].path = sound
    camera.components[components.audio_source].volume = 20
    camera.components[components.audio_source]:set()

    
    

    local bullet_position = camera.components[components.transform]:get_global_position(-0.3, -0.3, 0)
    local ray_start = camera.components[components.transform]:get_global_position(0, 0, 0)
    local ray_end = camera.components[components.transform]:get_global_position(0, 0, 1000)

    local hit = false
    local hit_info = {}
    hit, hit_info = raycast_3D(ray_start, ray_end)
    local target = deepcopy(ray_end)

    local normalize = function(vec3)
        local sun = math.abs(vec3.x) + math.abs(vec3.y) + math.abs(vec3.z)
        return { x = vec3.x / sun, y = vec3.y / sun, z = vec3.z / sun }
    end

    local spred_directions = {}
    if spred ~= 0 then
        for i = 1, quantity, 1 do
            spred_directions[i] = camera.components[components.transform]:get_local_direction((math.random() - 0.5) * spred,(math.random() - 0.5) * spred, 1 )
        end
    else
        for i = 1, quantity, 1 do
            spred_directions[i] = camera.components[components.transform]:get_local_direction(0,0,1)
        end
    end

    summon_bullet(bullet_position, normalize(ray_end), mesh, spred_directions, speed, life_time, damage, quantity, hit_scan, impulse,true,true,color,"")



    restart_arm_cannon_animation()
end

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
        local hit_object = game_object:new(hit_info.collision_object)

        if hit_object.components ~= nil and hit_object.components[components.lua_scripts] ~= nil and hit_object.components[components.lua_scripts]:has_script("game_scripts/mensage") and inputs.interact > 0 and inputs_last_frame.interact < 1 then
            hit_object.components[components.lua_scripts]:call_function("game_scripts/mensage", "interact", {})
        end
    end
end

shoot_timer = 0

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

        local get_valid_touches_top = function(objs_touching)
            local valid_touches = 0
            for key, value in pairs(objs_touching) do
                local obj_touching = game_object:new(value)
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
                local obj_touching = game_object:new(value)
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
        check_top.components[components.physics_3D]:get()
        hit_top = get_valid_touches_top(check_top.components[components.physics_3D].objs_touching) > 0

        --hit down
        check_down.components[components.transform]:change_position(pos.x, pos.y - 1.75, pos.z)
        check_down.components[components.physics_3D]:get()
        hit_down = get_valid_touches_down(check_down.components[components.physics_3D].objs_touching) > 1
        --print("AAAAA",#check_down.components[components.physics_3D].objs_touching,#check_down.components[components.physics_3D].colis_infos)



        if global_data:get("pause") < 1 then
            --interact
            if inputs.interact > 0 and inputs_last_frame.interact < 1 then
                interact()
            end

            --move camera
            window:get()
            camera_rotation.x = camera_rotation.x - (inputs.mouse_view_x) * mouse_sensitivity * 20
            camera_rotation.y = math.max(math.min(camera_rotation.y - ((inputs.mouse_view_y) * mouse_sensitivity * 20),
                90), -90)

            camera_rotation.x = camera_rotation.x - (inputs.analog_view_x) * mouse_sensitivity / 2.5
            camera_rotation.y = math.max(
                math.min(camera_rotation.y - ((inputs.analog_view_y) * mouse_sensitivity / 2.5), 90), -90)

            if not this_object_physics_3D_seted then
                this_object.components[components.physics_3D]:set()
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
            local input_dir = direction_reference.components[components.transform]:get_local_direction(inputs.foward, 0,
                -inputs.left)
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


            --move
            if hit_down and not (inpulse_y > 0) then
                impulse = { x = (move_dir.x * (speed + speed_boost)) + platform_movement.x * time.delta,
                    y = (move_dir.y * (speed + speed_boost)) + platform_movement.y * time.delta,
                    z = (move_dir.z * (speed + speed_boost)) + platform_movement.z * time.delta }
            else
                impulse = { x = (move_dir.x * (speed + speed_boost_air)) * time.sacale,
                    y = (move_dir.y * (speed + speed_boost_air)) + inpulse_y * time.sacale,
                    z = (move_dir.z * (speed + speed_boost_air)) * time.sacale }
            end
            this_object.components[components.physics_3D]:set_linear_velocity(impulse.x, impulse.y, impulse.z)

            if not hit_down then
                inpulse_y = inpulse_y + (time.delta * gravity.force.y)
            end

            --shoot
            if inputs.action_1 > 0 and inputs_last_frame.action_1 < 1 and shoot_timer <= 0 then
            --if inputs.action_1 > 0 then
                advanced_shoot({ file = "resources/3D Models/bullets.gltf", name = "round_bullet" },
                    "resources/Audio/sounds/shot_3.wav", 0.2, 50, 1, 10, 12,{r=1,g=0,b=0}, false)
                    shoot_timer = 0.2
            end
            shoot_timer = shoot_timer - time.delta

            --animate
            if animation_state.name == "normal" or animation_state.name == "jump" or animation_state.name == "walk" or animation_state.name == "" or animation_state.name == "open" or (animation_state.name == "recoil" and animation_state.finish) then
                if input_dir.x ~= 0 and input_dir.z ~= 0 and hit_down and not (inputs.action_2 > 0) then
                    start_arm_cannon_animation("walk", 1, true)
                else
                    if not (inputs.action_2 > 0) then
                        start_arm_cannon_animation("normal", 1, true)
                    end
                end
            end
        else
            this_object.components[components.physics_3D]:set_linear_velocity(0, 0, 0)
        end

        camera.components[components.transform]:change_rotation(-camera_rotation.y, 0, 0)
        this_object.components[components.transform]:change_rotation(0, camera_rotation.x, 0)
        pause_last_frame = global_data:get("pause") < 1

        this_object.components[components.physics_3D]:get()
    end

    play_arm_cannon_animation()
end

function COLLIDE(collision_info)
    --deepprint(collision_info)
end

function END()
    remove_object(camera.object_ptr)
    remove_object(check_top.object_ptr)
    remove_object(check_down.object_ptr)
    global_data:set("player_object_ptr", "")
end
