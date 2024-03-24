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

local mat = matreial:new()

local layers = {}
bullets_list = {}
bullets_data = {}
function bullets_data:new(obj, direction, spred, id, speed, life_time, damage, base_impulse, target, friendly, color,
                          behavior)
    return {
        object = obj,
        direction = direction,
        spred = spred,
        id = id,
        speed = speed,
        life_time = life_time,
        timer = life_time,
        damage = damage,
        base_impulse = base_impulse,
        target = target,
        friendly = friendly,
        behavior = behavior,
        color = color,
    }
end

--poly_mesh_object = {}

function add_bullet(data)
    table.insert(bullets_list, data)
end

function remove_bullet(adres)
    remove_object(bullets_list[adres].object.object_ptr)
    table.remove(bullets_list, adres)
end

function START()
    layers = global_data.layers


    mat.shader = "resources/Shaders/mesh"
    mat.textures[1] = "resources/Textures/white.png"
end

function UPDATE()
    time:get()

    

    for key, value in pairs(bullets_list) do

        bullets_list[key].timer = bullets_list[key].timer - (time.delta * time.scale)
        

        local bullet = bullets_list[key].object

        bullet.components.transform:get()
        local next_pos = (bullet.components.transform.position)

        next_pos.x = next_pos.x + bullets_list[key].base_impulse.x * time.delta * time.scale
        next_pos.y = next_pos.y + bullets_list[key].base_impulse.y * time.delta * time.scale
        next_pos.z = next_pos.z + bullets_list[key].base_impulse.z * time.delta * time.scale

        local id = bullets_list[key].id
        next_pos.x = next_pos.x + (bullets_list[key].spred[id].x * bullets_list[key].speed) * time.delta * time.scale
        next_pos.y = next_pos.y + (bullets_list[key].spred[id].y * bullets_list[key].speed) * time.delta * time.scale
        next_pos.z = next_pos.z + (bullets_list[key].spred[id].z * bullets_list[key].speed) * time.delta * time.scale

        next_pos.x = next_pos.x + (bullets_list[key].direction.x * bullets_list[key].speed) * time.delta * time.scale
        next_pos.y = next_pos.y + (bullets_list[key].direction.y * bullets_list[key].speed) * time.delta * time.scale
        next_pos.z = next_pos.z + (bullets_list[key].direction.z * bullets_list[key].speed) * time.delta * time.scale

        bullet.components.transform:change_position(next_pos.x, next_pos.y, next_pos.z)

        if bullets_list[key].timer <= 0 then
            
            remove_bullet(key)
        end
        

        --[[
        local objects_coliding = bullet.components[components.physics_3D]:get_objects_coliding()
        for key, value in pairs(objects_coliding) do
            print(value.object)
        end
        ]]
    end
    
end

function COLLIDE(collision_info)
end

function END()
end

function summon_bullet(args)
    --args.pos, args.mesh, args.spred, args.speed, args.life_time, args.damage, args.quantity, args.hit_scan,args.base_impulse,args.target,args.behavior





    --[[]]
    for i = 1, args.quantity, 1 do
        if args.hit_scan > 0 then
            
        else
            local bullet = game_object(create_object(layers.cenary))

            bullet.components.transform.is_ui = false
            bullet.components.transform.position = args.pos
            bullet.components.transform.rotation = { x = 0, y = 45, z = 0 }
            bullet.components.transform.scale = { x = 0.5, y = 0.5, z = 0.5 }
            bullet.components.transform.billboarding = 2
            bullet.components.transform:set()

            
            bullet.components.render_shader.layer = 2
            bullet.components.render_shader.vertex_size = 6
            local mat2 = deepcopy(mat)
            mat2.shader = "resources/Shaders/2D_bullet"
            mat2.color = { r = 1, g = 1, b = 1, a = 0.99 }
            mat2.textures[1] = "resources/Textures/energy_buble.svg"
            bullet.components.render_shader.material = mat2
            bullet.components.render_shader:set()
            
            bullet.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
            bullet.components[components.physics_3D].collision_shape = collision_shapes.sphere
            bullet.components[components.physics_3D].gravity_scale = 0
            bullet.components[components.physics_3D].collision_mesh = nil
            bullet.components[components.physics_3D].triger = true
            bullet.components[components.physics_3D].collision_layer = { layer = 10, layers_can_colide = { 9, } }
            bullet.components[components.physics_3D].scale = 0.25
            bullet.components[components.physics_3D].friction = 0
            bullet.components[components.physics_3D].get_collision_info = true
            bullet.components[components.physics_3D]:set()

            local bullet_data = bullets_data:new(bullet, args.direction, args.spred, i, args.speed, args.life_time,args.damage,args.base_impulse, args.target, args.friendly, args.color, args.behavior)
            add_bullet(bullet_data)
        end
    end

    return {}
end

function clear(args)
    while #bullets_list > 0 do
        remove_bullet(1)
    end
    return {}
end