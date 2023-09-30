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

require("short_cuts.create_collision")
require("short_cuts.create_mesh")

require("objects.global_data")


local layers = {}

bullets_data = {}
function bullets_data:new(obj, spred, speed, life_time, damage, impulse, target, behavior)
    return {
        obj = obj,
        spred = spred,
        speed = speed,
        life_time = life_time,
        timer = life_time,
        damage = damage,
        impulse = impulse,
        target = target,
        behavior = behavior,
    }
end

bullets_list = {}

function add_bullet(data)
    table.insert(bullets_list, data)
end

function remove_bullet(adres)
    table.remove(bullets_list, adres)
end

function START()
    layers = global_data:get_var("layers")
end

function UPDATE()
    time:get()


    for key, value in pairs(bullets_list) do
        bullets_list[key].timer = bullets_list[key].timer - time.delta
        if bullets_list[key].timer <= 0 then
            remove_bullet(key)
        end
    end
end

function COLLIDE(collision_info)
end

function END()
end

function summon_bullet(args)
    --args.pos, args.mesh, args.spred, args.speed, args.life_time, args.damage, args.quantity, args.hit_scan,args.base_impulse,args.target,args.behavior



    local mat = matreial:new()
    mat.shader = "resources/Shaders/mesh"
    mat.textures[1] = "resources/Textures/white.png"
    mat.color = { r = 1, g = 0, b = 0, a = 1 }

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

        --[[
        bullet:add_component(components.physics_3D)
        bullet.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
        bullet.components[components.physics_3D].collision_shape = collision_shapes.sphere
        bullet.components[components.physics_3D].gravity_scale = 0
        bullet.components[components.physics_3D].collision_mesh = nil
        bullet.components[components.physics_3D].triger = true
        bullet.components[components.physics_3D].collision_layer = { layer = 10, layers_can_colide = { 1, } }
        bullet.components[components.physics_3D].scale = 0.25
        bullet.components[components.physics_3D].friction = 0
        bullet.components[components.physics_3D]:set()
        ]]

        local bullet_data = bullets_data:new(bullet, args.spred, args.speed, args.life_time, args.damage,
            args.base_impulse, args.target, args.behavior)
        add_bullet(bullet_data)
    end

    return {}
end
