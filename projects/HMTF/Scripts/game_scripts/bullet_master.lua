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

update_per_type = {
    [bullet_types.ray] = function (bullet)
        
    end
}

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

local mesh_mat = matreial:new()
mesh_mat.shader = "mesh"
mesh_mat.textures[1] = "Textures/white.png"

local sprite_mat = matreial:new()
sprite_mat.shader = "sprite"
sprite_mat.textures[1] = "Textures/white.png"

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


start_per_type = {
    [bullet_types.ray] = function (bullet)
        print("ray")
        
        local bcomps = bullet.obj.components

        local start = bullet.start
        local target = bullet.target
        local dist = distance(start,target)

        
        local mp = midpoint(start, target)
        bcomps.transform:change_position(mp.x,mp.y,mp.z)

        bcomps.transform:look_at(bullet.target)
        bcomps.transform:change_scale(0.5,0.5,dist)

        print("start",start.x,start.y,start.z)
        print("target",target.x,target.y,target.z)

        local sb1 = game_object(create_object(bullet.obj.object_ptr)).components
        sb1.transform:change_rotation(90,0,0)
        sb1.render_shader.material = sprite_mat
        sb1.render_shader.material.color = bullet.color
        sb1.render_shader.material.color.a = sb1.render_shader.material.color.a - 0.001
        sb1.render_shader.layer = 2
        sb1.render_shader:set()

        local sb2 = game_object(create_object(bullet.obj.object_ptr)).components
        sb2.transform:change_rotation(0,90,0)
        sb2.render_shader.material = sprite_mat
        sb2.render_shader.material.color = bullet.color
        sb2.render_shader.material.color.a = sb2.render_shader.material.color.a - 0.001
        sb2.render_shader.layer = 2
        sb2.render_shader:set()
        
        
        
    end
}

function summon_bullet(args)

   

    local bullet_type = args[1]
    local group = args[2]
    local start = args[3]
    local target_direction_position = args[4]
    local speed = args[5]
    local color = args[6]
    local distance = args[7]
    local damage = args[8]
    local obj = game_object(create_object(layers.main))

    local bullet_data = {
        obj = obj,
        type = bullet_type,
        group = group,
        start = start,
        target = target_direction_position,
        speed = speed,
        color = color,
        distance = distance,
        damage = damage
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