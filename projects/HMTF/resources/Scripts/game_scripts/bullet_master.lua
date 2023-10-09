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

require("objects.global_data")

local mat = matreial:new()

local layers = {}
bullets_list = {}
bullets_data = {}
function bullets_data:new(obj,direction, spred,id, speed, life_time, damage, base_impulse, target,friendly, behavior)
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
        friendly=friendly,
        behavior = behavior,
    }
end



function add_bullet(data)
    table.insert(bullets_list, data)
end

function remove_bullet(adres)
    table.remove(bullets_list, adres)
end

function START()
    layers = global_data:get_var("layers")

    
    mat.shader = "resources/Shaders/mesh"
    mat.textures[1] = "resources/Textures/white.png"
    mat.color = { r = 1, g = 0, b = 0, a = 1 }
end

function UPDATE()
    time:get()

    --local timer = stopwatch:new()

    for key, value in pairs(bullets_list) do

        
        bullets_list[key].timer = bullets_list[key].timer - time.delta

        local bullet = bullets_list[key].object

        bullet.components[components.transform]:get()
        local next_pos = deepcopy(bullet.components[components.transform].position)

        next_pos.x = next_pos.x + bullets_list[key].base_impulse.x * time.delta
        next_pos.y = next_pos.y + bullets_list[key].base_impulse.y * time.delta
        next_pos.z = next_pos.z + bullets_list[key].base_impulse.z * time.delta

        local id = bullets_list[key].id
        next_pos.x = next_pos.x + (bullets_list[key].spred[id].x * bullets_list[key].speed) * time.delta
        next_pos.y = next_pos.y + (bullets_list[key].spred[id].y * bullets_list[key].speed) * time.delta
        next_pos.z = next_pos.z + (bullets_list[key].spred[id].z * bullets_list[key].speed) * time.delta

        next_pos.x = next_pos.x + (bullets_list[key].direction.x * bullets_list[key].speed) * time.delta
        next_pos.y = next_pos.y + (bullets_list[key].direction.y * bullets_list[key].speed) * time.delta
        next_pos.z = next_pos.z + (bullets_list[key].direction.z * bullets_list[key].speed) * time.delta

        bullet.components[components.transform]:change_position(next_pos.x,next_pos.y,next_pos.z)

        if bullets_list[key].timer <= 0 then
            remove_object(bullets_list[key].object.object_ptr)
            remove_bullet(key)
        end

        
        bullet.components[components.physics_3D]:get()
        for key, value in pairs(bullet.components[components.physics_3D].colis_infos) do
            --print(value.object)
        end
        
    end
    
    --print(timer:getTime())

end

function COLLIDE(collision_info)
end

function END()
end

function summon_bullet(args)
    --args.pos, args.mesh, args.spred, args.speed, args.life_time, args.damage, args.quantity, args.hit_scan,args.base_impulse,args.target,args.behavior



    

    --[[]]
    for i = 1, args.quantity, 1 do
        local bullet = game_object:new(create_object(layers.cenary))

        bullet:add_component(components.transform)
        bullet.components[components.transform].position = deepcopy(args.pos)
        bullet.components[components.transform].scale = { x = 0.25, y = 0.25, z = 0.25 }
        bullet.components[components.transform]:set()

        bullet:add_component(components.render_mesh)
        bullet.components[components.render_mesh].layer = 2
        bullet.components[components.render_mesh].meshes_cout = 1
        bullet.components[components.render_mesh].meshes = deepcopy({ args.mesh })
        bullet.components[components.render_mesh].materials = deepcopy({ mat })
        bullet.components[components.render_mesh]:set()

        
        bullet:add_component(components.physics_3D)
        bullet.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
        bullet.components[components.physics_3D].collision_shape = collision_shapes.sphere
        bullet.components[components.physics_3D].gravity_scale = 0
        bullet.components[components.physics_3D].collision_mesh = nil
        bullet.components[components.physics_3D].triger = true
        bullet.components[components.physics_3D].get_collision_data = true
        bullet.components[components.physics_3D].collision_layer = { layer = 10, layers_can_colide = { 1, } }
        bullet.components[components.physics_3D].scale = 0.25
        bullet.components[components.physics_3D].friction = 0
        bullet.components[components.physics_3D]:set()

        
        
        local bullet_data = bullets_data:new(bullet,args.direction, args.spred,i, args.speed, args.life_time, args.damage,
            args.base_impulse, args.target,args.friendly, args.behavior)
        add_bullet(bullet_data)
    end

    return {}
end
