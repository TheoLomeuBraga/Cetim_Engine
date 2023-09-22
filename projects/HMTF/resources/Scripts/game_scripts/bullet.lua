require("engine_libs.definitions")
require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")

require("engine_libs.objects.game_object")
require("engine_libs.objects.time")

require("engine_libs.objects.material")
require("engine_libs.objects.global_data")
require("engine_libs.objects.vectors")
require("engine_libs.objects.gravity")
require("engine_libs.objects.scene_3D")

require("engine_libs.short_cuts.create_collision")
require("engine_libs.short_cuts.create_mesh")

local this_object = {}

damage = 1
direction = { x = 1, y = 0, z = 0 }
speed = 1
special_pattern = ""
max_distance = -1
life_time = 1
mesh = {
    file = "",
    name = ""
}


function START()
    this_object = game_object:new(this_object_ptr)

    if mesh.file ~= "" then
        local mat = matreial:new()
        mat.shader = "resources/Shaders/mesh"
        mat.textures[1] = "resources/Textures/white.png"
        this_object:add_component(components.render_mesh)
        this_object.components[components.render_mesh].layer = 2
        this_object.components[components.render_mesh].meshes_cout = 1
        this_object.components[components.render_mesh].meshes = deepcopy({ mesh })
        this_object.components[components.render_mesh].materials = deepcopy({ mat })
        this_object.components[components.render_mesh]:set()

        this_object.components[components.transform]:get()

        this_object:add_component(components.physics_3D)
        this_object.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
        this_object.components[components.physics_3D].collision_shape = collision_shapes.convex
        this_object.components[components.physics_3D].gravity_scale = 0
        this_object.components[components.physics_3D].collision_mesh = deepcopy( mesh )
        this_object.components[components.physics_3D].triger = true
        this_object.components[components.physics_3D].scale = this_object.components[components.transform].scale
        this_object.components[components.physics_3D].friction = 0
        this_object.components[components.physics_3D]:set()
    end



    --this_object:add_component(components.physics_3D)
    --this_object.components[components.physics_3D]:set()
end

function UPDATE()
    time:get()

    --print(direction.x * speed * time.sacale,direction.y * speed * time.sacale,direction.z * speed * time.sacale)
    --this_object.components[components.physics_3D]:set_linear_velocity(direction.x * speed,direction.y * speed,direction.z * speed)

    this_object.components[components.transform]:get()
    local next_pos = {x=this_object.components[components.transform].position.x + (direction.x * speed * time.delta),y=this_object.components[components.transform].position.y + (direction.y * speed * time.delta),z=this_object.components[components.transform].position.z + (direction.z * speed * time.delta)}
    this_object.components[components.transform]:change_position(next_pos.x,next_pos.y,next_pos.z)

    life_time = life_time - time.delta
    if life_time <= 0 then
        remove_object(this_object_ptr)
    end
end

function COLLIDE(collision_info)
end

function END()
end
