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

local this_object = {}

damage = 1
direction = { x = 1, y = 0, z = 0 }
base_inpulse = { x = 0, y = 0, z = 0 }
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
        mat.color = { r = 1, g = 0, b = 0, a = 1 }
        this_object:add_component(components.render_mesh)
        this_object.components[components.render_mesh].layer = 2
        this_object.components[components.render_mesh].meshes_cout = 1
        this_object.components[components.render_mesh].meshes = deepcopy({ mesh })
        this_object.components[components.render_mesh].materials = deepcopy({ mat })
        this_object.components[components.render_mesh]:set()

        time:get()
        this_object.components[components.transform]:get()
        local pos = deepcopy(this_object.components[components.transform].position)
        local next_pos = { x = pos.x + (direction.x * speed * time.delta ) + (base_inpulse.x * time.delta), y = pos.y + (direction.y * speed * time.delta) + (base_inpulse.y * time.delta),z = pos.z + (direction.z * speed * time.delta) + (base_inpulse.z * time.delta) }
        this_object.components[components.transform]:change_position(next_pos.x, next_pos.y, next_pos.z)

        
        this_object:add_component(components.physics_3D)
        this_object.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
        this_object.components[components.physics_3D].collision_shape = collision_shapes.sphere
        this_object.components[components.physics_3D].gravity_scale = 0
        this_object.components[components.physics_3D].collision_mesh = nil
        this_object.components[components.physics_3D].triger = true
        this_object.components[components.physics_3D].collision_layer = { layer = 10, layers_can_colide = { 1, } }
        this_object.components[components.physics_3D].scale = 0.25
        this_object.components[components.physics_3D].friction = 0
        this_object.components[components.physics_3D]:set()
        --[[]]

        
    end
end

function UPDATE()
    time:get()

    
    this_object.components[components.transform]:get()
    local pos =  deepcopy(this_object.components[components.transform].position)
    local next_pos = {x=(pos.x + (direction.x * speed * time.delta)) + (base_inpulse.x * time.delta),y=(pos.y + (direction.y * speed * time.delta)) + (base_inpulse.y * time.delta),z=(pos.z + (direction.z * speed * time.delta)) + (base_inpulse.z * time.delta)}
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
