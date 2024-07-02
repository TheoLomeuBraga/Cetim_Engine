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

--[[
    obj
    type
    group
    start
    target_direction
    speed
    color
]]

function distance(a, b)
    local dx = b.x - a.x
    local dy = b.y - a.y
    local dz = b.z - a.z
    return math.sqrt(dx*dx + dy*dy + dz*dz)
end

function midpoint(a, b)
    local mx = (a.x + b.x) / 2
    local my = (a.y + b.y) / 2
    local mz = (a.z + b.z) / 2
    return {x = mx, y = my, z = mz}
end

function normalize(vector)
    local length = math.sqrt(vector.x^2 + vector.y^2 + vector.z^2)
    return {
        x = vector.x / length,
        y = vector.y / length,
        z = vector.z / length
    }
end

function calculate_next_position(a, b, t)
    -- Calcula o vetor direção do ponto a para o ponto b
    local direction = {x = b.x - a.x, y = b.y - a.y, z = b.z - a.z}
    
    -- Normaliza o vetor direção
    local normalized_direction = normalize(direction)
    
    -- Calcula a próxima posição do objeto
    local next_position = {
        x = a.x + normalized_direction.x * t,
        y = a.y + normalized_direction.y * t,
        z = a.z + normalized_direction.z * t
    }
    
    return next_position
end


update_per_type = {

}

function UPDATE()
    time:get()
    
    for index, value in pairs(bullet_list) do
        update_per_type[value.type](value)
        if value.dead then
            remove_object(value.obj.object_ptr)
            table.remove(bullet_list,index)
        end
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

local mesh_mat = matreial:new()
mesh_mat.shader = "mesh"
mesh_mat.textures[1] = "Textures/white.png"

local sprite_mat = matreial:new()
sprite_mat.shader = "sprite"
sprite_mat.textures[1] = "Textures/white.png"




start_per_type = {
    
}

function summon_bullet(args)

   

    local type = args[1]
    local start = args[2]
    local target = args[3]
    local extra = args[4]

    local obj = game_object(create_object(layers.main))

    local bullet_data = {
        obj = obj,
        type = type,
        start = start,
        target = target,
        extra = extra
    }

    start_per_type[bullet_data.type](bullet_data)

    table.insert(bullet_list,bullet_data)

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