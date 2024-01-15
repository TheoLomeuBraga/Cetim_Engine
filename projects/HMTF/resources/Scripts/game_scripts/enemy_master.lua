require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_mesh")
require("short_cuts.create_collision")
require("resources.playable_scene")



enemys_list = {}

function START()
end

function UPDATE()
    local update_entity_map = {
        test_enemy = function(enemy)

        end,
    }
    for index, value in ipairs(enemys_list) do
        update_entity_map[value.type](value)
        --print(value.type)
    end
end

function COLLIDE(collision_info)
end

function END()
end

function summon_enemy(args)
    local pos = args.pos
    local rot_y = args.rot_y
    local type = args.type

    local obj = game_object(create_object(global_data.layers.main))
    obj.components.transform.position = deepcopy(pos)
    --print("AAAAA",pos.x,pos.y,pos.z)
    obj.components.transform.rotation = { x = 0, y = rot_y, z = 0 }
    obj.components.transform:set()

    local enemy = {
        type = type,
        obj = obj,
        rig_obj = nil,
        parts_ptr_list = nil,
        animation = "",
        animation_time = 0,
    }

    local actions_per_type = {
        test_enemy = function()
            local model_path = "resources/3D Models/test_enimy.gltf"

            local enemy_physics_3D = enemy.obj.components.physics_3D
            enemy_physics_3D.boady_dynamic = boady_dynamics.kinematic
            enemy_physics_3D.collision_shape = collision_shapes.capsule
            enemy_physics_3D.scale = Vec3:new(1, 2, 1)
            enemy_physics_3D.rotate_x = false
            enemy_physics_3D.rotate_y = false
            enemy_physics_3D.rotate_z = false
            enemy_physics_3D.friction = 100
            enemy_physics_3D.gravity_scale = 0
            enemy_physics_3D.triger = false
            enemy_physics_3D:set()

            local enemy_data = get_scene_3D(model_path)
            local enemy_structures = cenary_builders.entity(enemy.obj.object_ptr, 2, enemy_data, "resources/Shaders/mesh",
                true, false)
            
            enemy.rig_obj = enemy_structures.obj
            enemy.rig_obj.components.transform.position.y = -1.5
            enemy.rig_obj.components.transform:set()
            enemy.parts_ptr_list = enemy_structures.parts_ptr_list
        end,
    }

    actions_per_type[type]()



    table.insert(enemys_list, enemy)

    return {}
end

function remove_enemy(adres)
    
    table.remove(enemys_list, adres)
end

function clear(args)
    while #enemys_list > 0 do
        remove_enemy(1)
    end
    return {}
end
