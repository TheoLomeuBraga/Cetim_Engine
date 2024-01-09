require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_mesh")
require("short_cuts.create_collision")
require("resources.playable_scene")



enemys = {}

function START()
end

function UPDATE()
    local update_entity_map = {
        test_enemy = function (enemy)
            
        end,
    }
    for index, value in ipairs(enemys) do
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
    print("AAAAA",pos.x,pos.y,pos.z)
    obj.components.transform.rotation = {x=0,y=rot_y,z=0}
    obj.components.transform:set()

    local enemy = {
        type = type,
        obj = obj,
        rig_obj = nil,
        animation = "",
        animation_time = 0,
    }

    local actions_per_type = {
        test_enemy = function ()
            local model_path = "resources/3D Models/test_enimy.gltf"

            local enemy_data = get_scene_3D(model_path)
            local enemy_structures = cenary_builders.entity(obj.object_ptr, 4, enemy_data, "resources/Shaders/mesh", true, false)
            --enemy.rig_obj = enemy_structures.parts_list
            
        end,
    }

    actions_per_type[type]()

    

    table.insert(enemys,enemy)

    return {}
end
