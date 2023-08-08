require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.game_object")
require("TMP_libs.objects.input")
require("TMP_libs.objects.time")
require("TMP_libs.objects.gravity")
require("TMP_libs.objects.global_data")
require("TMP_libs.objects.window")
require("TMP_libs.short_cuts.create_collision")
require("TMP_libs.objects.scene_3D")
require("TMP_libs.short_cuts.create_mesh")
require("math")

layers = {}
charter_type = ""
charter_size = { x = 1, y = 1, z = 1 }

local test_rc_obj = {}

local this_object = {}
local camera_man_object = {}

local detect_top = {}
local detect_down = {}

local hit_top = false
local hit_down = false



local control_last_frame = {
    left = false,
    right = false,
    top = false,
    down = false,
    jump = false,
    action = false,
}

local control = {
    left = false,
    right = false,
    top = false,
    down = false,
    jump = false,
    action = false,
}


function get_control()
    if camera_man_object.components[components.lua_scripts]:get_variable("game_scripts/free_camera", "is_free") then
        control.top = false
        control.down = false
        control.left = false
        control.right = false
        control.jump = false
        control.action = false
        control_last_frame = deepcopy(control)
    else
        control_last_frame = deepcopy(control)
        control.top = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.w]) > 0
        control.down = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.s]) > 0
        control.left = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.a]) > 0
        control.right = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.d]) > 0
        control.jump = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.space]) > 0
        control.action = keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.left]) > 0
    end
end

armature_data = {
    ceane_data = {},
    object_list = {},
    object_list_ptr = {}
}

function create_test_cube(pos)
    local mat = matreial:new()
    mat.shader = "resources/Shaders/mesh"
    mat.textures[1] = "resources/Textures/white.png"
    return create_mesh(layers.cenary,false,pos,{x=0,y=0,z=0},{x=1,y=1,z=1},2,{mat},{mesh_location:new("resources/Levels/3D/test_level.gltf","Cube.003")})

end

function START()
    camera_man_object = game_object:new(global_data:get_var("camera_object_ptr"))

    this_object = game_object:new(this_object_ptr)

    if charter_type == "2D" then
        this_object.components[components.transform]:get()
        local pos = deepcopy(this_object.components[components.transform].position)

        detect_top = create_collision_2D(layers.cenary, Vec3:new(pos.x, pos.y + (charter_size.y / 2), pos.z),
            Vec3:new(0, 0, 0), Vec3:new(charter_size.x - 0.1, 0.05, charter_size.y), false, collision_shapes.box, nil,
            true)

        detect_down = create_collision_2D(layers.cenary, Vec3:new(pos.x, pos.y - (charter_size.y / 2), pos.z),
            Vec3:new(0, 0, 0), Vec3:new(charter_size.x - 0.1, 0.05, charter_size.y), false, collision_shapes.box, nil,
            true)

        this_object.components[components.physics_2D].rotate = false
        this_object.components[components.physics_2D].gravity_scale = 0
        this_object.components[components.physics_2D].friction = 0
        this_object.components[components.physics_2D]:set()

        --[[
        local mat  = matreial:new()
        mat.shader = "resources/Shaders/sprite"
        test_rc_obj:add_component(components.render_sprite)
        test_rc_obj.components[components.render_sprite].material = deepcopy(mat)
        test_rc_obj.components[components.render_sprite].layer = 2
        test_rc_obj.components[components.render_sprite].selected_tile = 4
        test_rc_obj.components[components.render_sprite].tile_set_local = "resources/Levels/2D/tile_set.json"
        test_rc_obj.components[components.render_sprite]:set()
        ]]
        

    elseif charter_type == "3D" then

        armature_data.object_list = {}
        for key, value in pairs(armature_data.object_list_ptr) do
            armature_data.object_list[key] = game_object:new(value)
        end

        this_object.components[components.transform]:get()
        local pos = deepcopy(this_object.components[components.transform].position)

        detect_top = create_collision_3D(layers.cenary, Vec3:new(pos.x, pos.y + (charter_size.y / 2), pos.z),
            Vec3:new(0, 0, 0), Vec3:new(0.1, 0.5, 0.1), true, collision_shapes.cube, nil,
            true)

        detect_down = create_collision_3D(layers.cenary, Vec3:new(pos.x, pos.y - (charter_size.y / 2), pos.z),
            Vec3:new(0, 0, 0), Vec3:new(0.1, 0.5, 0.1), true, collision_shapes.cube, nil,
            true)

        this_object.components[components.physics_3D].rotate_X = 0
        this_object.components[components.physics_3D].rotate_Y = 0
        this_object.components[components.physics_3D].rotate_Z = 0
        this_object.components[components.physics_3D].friction = 0
        this_object.components[components.physics_3D]:set()

        --test_rc_obj = create_test_cube(Vec3:new(0,0,0))

    end
end

function get_floor_cealing_hit()
    this_object.components[components.transform]:get()
    local pos = deepcopy(this_object.components[components.transform].position)

    if charter_type == "2D" then
        detect_top.components[components.transform]:change_position(pos.x, pos.y + (charter_size.y / 2) + 0.05, pos.z)
        detect_top.components[components.physics_2D]:get()
        hit_top = tablelength(detect_top.components[components.physics_2D].objs_touching) > 1

        detect_down.components[components.transform]:change_position(pos.x, pos.y - (charter_size.y / 2) - 0.05, pos.z)
        detect_down.components[components.physics_2D]:get()
        hit_down = tablelength(detect_down.components[components.physics_2D].objs_touching) > 1
    elseif charter_type == "3D" then
        detect_top.components[components.transform]:change_position(pos.x, pos.y + (charter_size.y / 2) + 0.5, pos.z)
        detect_top.components[components.physics_3D]:get()
        hit_top = tablelength(detect_top.components[components.physics_3D].objs_touching) > 1

        detect_down.components[components.transform]:change_position(pos.x, pos.y - (charter_size.y / 2) - 0.5, pos.z)
        detect_down.components[components.physics_3D]:get()
        hit_down = tablelength(detect_down.components[components.physics_3D].objs_touching) > 1

        this_object.components[components.physics_3D]:get()
    end
end

function set_sprite(id)
    this_object.components[components.render_sprite].selected_tile = id
    this_object.components[components.render_sprite]:set()
end



local last_direction_2D = 1

function action()
    if charter_type == "2D" then
        this_object.components[components.transform]:get()
        local pos = deepcopy(this_object.components[components.transform].position)
        local target_pos = { x = pos.x + (last_direction_2D * 10), y = pos.y }

        local hit, hit_data = raycast_2D({ x = pos.x + (last_direction_2D * 1.01), y = pos.y }, target_pos)

        if hit then
            local hit_obj = game_object:new(hit_data.collision_object)
            hit_obj.components[components.physics_2D]:get()

            if hit_obj.components[components.physics_2D].boady_dynamic == boady_dynamics.dynamic then
                hit_obj.components[components.physics_2D]:add_impulse(0, 1000)
            end
        end
    elseif charter_type == "3D" then

        this_object.components[components.transform]:get()
        local pos = deepcopy(this_object.components[components.transform].position)
        local target = this_object.components[components.transform]:get_global_position(50,0,0)

       

        local hit = false  
        local hit_data = {}
        hit , hit_data = raycast_3D(pos,Vec3:new(target.x,target.y,target.z))
        
        if hit then
            local hit_obj = game_object:new(hit_data.collision_object)
            hit_obj.components[components.physics_3D]:get()

            if hit_obj.components[components.physics_3D].boady_dynamic == boady_dynamics.dynamic then
                hit_obj.components[components.physics_3D]:add_impulse(0, 50,0)
            end

            --test_rc_obj.components[components.transform]:change_position(hit_data.position.x,hit_data.position.y,hit_data.position.z)

        end
        
    end
end

local movement = Vec3:new(0, 0, 0)

local animation_selected_last_frame = "walk"
local animation_selected = "walk"
local animation_advancement = 0

function play_3D_animation()
    if hit_down then
        if control.top or control.down or control.left or control.right then
            animation_selected = "walk"
        else
            animation_selected = "stop"
        end
        
    else 
        animation_selected = "jump"
    end

    if animation_selected_last_frame ~= animation_selected then
        animation_advancement = 0
    end

    animation_advancement = animation_advancement + ((time.delta / armature_data.ceane_data.animations[animation_selected].duration) * 2)

    if animation_advancement > 1 then
        if animation_selected == "walk" then
            animation_advancement = 0
        elseif animation_selected == "stop" then
            animation_advancement = 0
        elseif animation_selected == "jump" then
            animation_advancement = 1
        end
    end

    --print(animation_advancement)

    --tablelength(armature_data.ceane_data.animations[animation_selected].key_frames)

    local selected_frame = math.floor(animation_advancement * tablelength(armature_data.ceane_data.animations[animation_selected].key_frames))
    if selected_frame == 0 then
        selected_frame = 1
    end

    apply_key_frame(armature_data.object_list,armature_data.ceane_data.animations[animation_selected].key_frames[selected_frame])

    animation_selected_last_frame = animation_selected

end

local speed = 7
local y_power = 10



function UPDATE()
    gravity:get()
    time:get()


    get_control()

    get_floor_cealing_hit()

    if control.action and not control_last_frame.action then
        action()
    end

    if charter_type == "2D" then
        if movement.x < 0 then
            set_sprite(4)
        elseif movement.x > 0 then
            set_sprite(3)
        elseif movement.y > 0 then
            set_sprite(2)
        elseif movement.y < 0 then
            set_sprite(5)
        else
            set_sprite(1)
        end

        --movement

        if control.jump and not control_last_frame.jump and hit_down then
            movement.y = y_power
        end

        if hit_top and movement.y > 0 then
            movement.y = 0
        end
        movement.y = movement.y + (gravity.force.y * time.delta)
        if hit_down and movement.y < 0 then
            movement.y = 0
        end

        if control.left then
            movement.x = -speed
            last_direction_2D = -1
        elseif control.right then
            movement.x = speed
            last_direction_2D = 1
        else
            movement.x = 0
        end

        this_object.components[components.physics_2D]:set_linear_velocity(movement.x, movement.y)

    elseif charter_type == "3D" then
        local mouse_rot = camera_man_object.components[components.lua_scripts]:get_variable("game_scripts/free_camera",
            "current_rot")
        this_object.components[components.transform]:change_rotation(0, mouse_rot.x + 90, 0)

        --movement

        if control.jump and not control_last_frame.jump and hit_down then
            movement.y = y_power
        end
        movement.y = movement.y + (gravity.force.y * time.delta)
        if hit_down and movement.y < 0 then
            movement.y = 0
        end

        if control.left then
            movement.z = -speed
        elseif control.right then
            movement.z = speed
        else
            movement.z = 0
        end

        if control.top then
            movement.x = speed
        elseif control.down then
            movement.x = -speed
        else
            movement.x = 0
        end

        local local_direction = this_object.components[components.transform]:get_local_direction(movement.x, movement.y,movement.z)

        this_object.components[components.physics_3D]:set_linear_velocity(local_direction.x, movement.y,local_direction.z)
        
        play_3D_animation()

        --[[
        local test = create_collision_3D(layers.cenary, Vec3:new(0,0,0),Vec3:new(0, 0, 0), Vec3:new(1, 1, 1), true, collision_shapes.cube, nil,false)
        print(test.object_ptr)
        remove_object(test.object_ptr)
        ]]

    end
end

function END()
    if charter_type == "2D" then
        remove_object(detect_top.object_ptr)
        detect_top = nil

        remove_object(detect_down.object_ptr)
        detect_down = nil
    elseif charter_type == "3D" then
        remove_object(detect_top.object_ptr)
        detect_top = nil

        remove_object(detect_down.object_ptr)
        detect_down = nil
    end
end

function COLLIDE(collision_info)
    if charter_type == "2D" then

    elseif charter_type == "3D" then

    end
end
