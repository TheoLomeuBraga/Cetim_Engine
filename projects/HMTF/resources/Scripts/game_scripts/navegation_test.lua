register_function_set("debug")

require("engine_libs.function_sets.navmesh")
require("engine_libs.short_cuts.create_mesh")

require("objects.time")



this_object = nil

walker_cube = {}

local mat = nil

path = {}
last_progression = 0
last_progression_2_ptr = {0}

function update_progression()
    walker_cube.components.transform:get()
    local pos = walker_cube.components.transform.position

    path = generate_navmesh_path(pos, { x = 67.0, y = 80.5, z = -296.0 })
    
    last_progression_2_ptr[1] = 0
end

function START()
    --path = generate_navmesh_path({x=-21, y=40.5, z=-138},{x=67.0, y=68.5, z=-296.0},"")
    path = generate_navmesh_path({ x = -21, y = 40.5, z = -138 }, { x = 67.0, y = 80.5, z = -296.0 })

    mat = matreial:new()
    mat.shader = "resources/Shaders/mesh"
    mat.textures = { "resources/Textures/white.png" }
    mat.color = { r = 1, g = 0, b = 1, a = 1 }
    walker_cube = create_mesh(this_object_ptr, false, path[1], { x = 0, y = 0, z = 0 }, { x = 1, y = 1, z = 1 }, 2, { mat },
        { mesh_location:new("resources/3D Models/visual_debug_shapes.gltf", "Cube") })

    for key, value in pairs(path) do
        local point = deepcopy(value)
        point.y = point.y + 2
        mat.color = { r = 0, g = 0, b = 1, a = 1 }
        create_mesh(this_object_ptr, false, point, { x = 0, y = 0, z = 0 }, { x = 0.25, y = 2, z = 0.25 }, 2, { mat },
            { mesh_location:new("resources/3D Models/visual_debug_shapes.gltf", "Cube") })
    end
end

update_progression_time = 0

local color_no = 0

function UPDATE()
    

    if last_progression ~= nil then
        local walk_ret = walk_along_the_path(path, last_progression, 1)

        if walk_ret == nil then
            last_progression = nil
        else
            if mat.color.r < 1 then
                mat.color.r = mat.color.r + 0.5
            elseif mat.color.g < 1 then
                mat.color.r = 0
                mat.color.g = mat.color.g + 0.5
            elseif mat.color.b < 1 then
                mat.color.g = 0
                mat.color.b = mat.color.b + 0.5
            else
                mat.color = { r = 0, g = 0, b = 0, a = 1 }
            end

            if color_no >= 6 then
                color_no = 0
            end

            if walk_ret.rotation == nil then
                walk_ret.rotation = { x = 0, y = 0, z = 0 }
            end
            create_mesh(this_object_ptr, false, walk_ret.position, walk_ret.rotation, { x = 0.2, y = 0.2, z = 1 }, 2,
                { mat }, { mesh_location:new("resources/3D Models/visual_debug_shapes.gltf", "Cube") })
            last_progression = walk_ret.progression
        end
    end

    

    
    time:get()
    walk_to(walker_cube,path,last_progression_2_ptr, 1,  5 * time.scale * time.delta,true)

    

    if update_progression_time == 100 then
        update_progression()
        update_progression_time = 0
    else
        update_progression_time = update_progression_time + 1
    end
end

function COLLIDE(collision_info)
end

function END()
end
