require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_mesh")
require("short_cuts.create_collision")


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
        print(value.type)
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
    obj.components.transform.rotation = {x=0,y=rot_y,z=0}
    obj.components.transform:set()

    local actions_per_type = {
        test_enemy = function ()
            local model_path = "resources/3D Models/test_enimy.gltf"
        end,
    }

    actions_per_type[type]()

    local enemy = {
        type = type,
        obj = obj,
        animation = "",
        animation_time = 0,
    }

    table.insert(enemys,enemy)

    return {}
end
