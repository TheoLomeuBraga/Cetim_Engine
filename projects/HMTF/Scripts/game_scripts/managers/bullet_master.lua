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
    return math.sqrt(dx * dx + dy * dy + dz * dz)
end

function midpoint(a, b)
    local mx = (a.x + b.x) / 2
    local my = (a.y + b.y) / 2
    local mz = (a.z + b.z) / 2
    return { x = mx, y = my, z = mz }
end

function normalize(vector)
    local length = math.sqrt(vector.x ^ 2 + vector.y ^ 2 + vector.z ^ 2)
    return {
        x = vector.x / length,
        y = vector.y / length,
        z = vector.z / length
    }
end

function calculate_next_position(a, b, t)
    -- Calcula o vetor direção do ponto a para o ponto b
    local direction = { x = b.x - a.x, y = b.y - a.y, z = b.z - a.z }

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

start_per_type = {
    ["floor"] = function(data)
        local mat = matreial:new("mesh")
        mat.textures[1] = "Textures/white.png"
        data.obj = create_mesh(create_object(), false, midpoint(data.start, data.target), { x = 0, y = 0, z = 0 }, { x = 2, y = 2, z = 2 }, 2,{ mat }, { mesh_location:new("engine assets/engine_models.glb", "oclusion_box:0") }, false)
        data.obj.components.physics_3D.collision_mesh = mesh_location:new("engine assets/engine_models.glb", "oclusion_box:0")
        data.obj.components.physics_3D:set()
        data.timer = 5
    end,
    ["normal"] = function(data)
        local mat = matreial:new("mesh")
        mat.textures[1] = "Textures/white.png"
        data.obj = create_mesh(create_object(), false, data.start, { x = 0, y = 0, z = 0 }, { x = 0.2, y = 0.2, z = 0.2 }, 2,{ mat }, { mesh_location:new("3D Models/new_bullets.glb", "magic projectile:0") }, false)

        data.timer = 5
        data.progresion = 0
    end,
    ["impact"] = function(data)
        local mat = matreial:new("explosion_mesh")
        mat.textures[1] = "Textures/white.png"

        local mesh = mesh_location:new("3D Models/new_bullets.glb", "magic projectile:0")
        if data.mesh ~= nil then
            mesh = data.mesh
        end
        data.obj = create_mesh(create_object(), false, data.target, { x = 0, y = 0, z = 0 }, { x = 0.2, y = 0.2, z = 0.2 }, 2,{ mat }, { mesh }, false)
        
        data.obj.components.render_mesh.render_count = 256
        data.obj.components.render_mesh.materials[1].inputs.progresion = 1
        data.obj.components.render_mesh:set()

        data.timer = 0.1
        data.progresion = 0
    end,
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



    
    if start_per_type[bullet_data.type] ~= nil then
        start_per_type[bullet_data.type](bullet_data)
        
        table.insert(bullet_list, bullet_data)
    end


    

    return {}
end

update_per_type = {
    ["floor"] = function(data)
        if data.timer < 0 then
            data.dead = true
        end
        data.timer = data.timer - time.delta
    end,
    ["normal"] = function(data)
        
        data.progresion = data.progresion + (time.delta * 50)

        data.obj.components.transform:get()
        local previous_position = data.obj.components.transform.position
        
        local next_pos = calculate_next_position(data.start, data.target, data.progresion)
        data.obj.components.transform:change_position(next_pos.x,next_pos.y,next_pos.z)
        data.obj.components.transform.position = next_pos
        data.obj.components.transform:set()

        if data.timer < 0 then
            data.dead = true
        end
        data.timer = data.timer - time.delta

        local hit = false
        local hit_info = nil
        hit,hit_info = raycast_3D(previous_position, next_pos)
        if hit then
            summon_bullet({"impact",hit_info.position,hit_info.position,nil})
            data.dead = true
        end

    end,
    ["impact"] = function(data)
        
        data.progresion = data.progresion + (time.delta * 50)

        data.obj.components.transform:get()
        local previous_position = data.obj.components.transform.position
        
        

        if data.timer < 0 then
            data.dead = true
        end
        data.timer = data.timer - time.delta

        data.obj.components.render_mesh.materials[1].inputs.progresion = data.obj.components.render_mesh.materials[1].inputs.progresion + (time.delta*50)
        data.obj.components.render_mesh:set()

       

    end,
}

function UPDATE()
    time:get()
    
    
    for index, value in pairs(bullet_list) do
        if update_per_type[value.type] ~= nil then
            update_per_type[value.type](value)
        end

        if value.dead then
            remove_object(value.obj.object_ptr)
            table.remove(bullet_list, index)
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








function clean(args)
    for index, value in pairs(bullet_list) do
        remove_object(value.obj.object_ptr)
    end
    entitys_list = {}
    target_points = {}
    return {}
end
