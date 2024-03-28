require("definitions")
require("components.extras")
require("components.component_all")
require("components.component_index")

require("objects.game_object")
require("objects.time")

require("objects.material")
require("objects.global_data")
require("objects.vectors")
require("objects.gravity")
require("objects.scene_3D")
require("objects.time")

require("short_cuts.create_collision")
require("short_cuts.create_mesh")




bullet_list = {}

target_points = {}

local bullet_types = {
    ray = 0,
    fast = 1,
    big = 2,
    missile = 3,

    prediction_ray = 4,
    prediction_fast = 5,
    prediction_big = 6,
    prediction_missile = 7,
}

local bullet_groups = {
    friend = 0,
    foe = 1,
    hazard = 2
}

layers = nil
function START()
    layers = global_data.layers

end

update_per_type = {}

function UPDATE()
    time:get()
    
    for index, value in ipairs(bullet_list) do
        update_per_type[value.type](value)
    end

end

function COLLIDE(collision_info)
end

function END()
end

function set_target_point(args)
    target_points[args[1]] = args[2]
    return {}
end

function remove_target_point(args)
    target_points[args[1]] = nil
    return {}
end

start_per_type = {}

function summon_bullet(args)

    local bullet_type = args[1]
    local group = args[2]
    local start = args[3]
    local target_direction = args[4]
    local speed = args[5]
    local obj = game_object(create_object(layers.main))

    local bullet_data = {
        obj = obj,
        type = bullet_type,
        group = group,
        start = start,
        target_direction = target_direction,
        speed = speed,
    }

    table.insert(start_per_type,bullet_data)

    return {}
end

function clean(args)
    for index, value in pairs(bullet_list) do
        remove_object(value.obj.object_ptr)
    end
    entitys_list = {}
    target_points = {}
    return {}
end