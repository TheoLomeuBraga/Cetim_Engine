register_function_set("debug")

require("engine_libs.function_sets.navmesh")
require("engine_libs.short_cuts.create_mesh")
require("components.component_all")
require("objects.global_data")
require("objects.time")



this_object = nil

path = {}

walker_cube = {}

local mat = nil

function START()
    

    path = generate_navmesh_path({x=-21, y=40.5, z=-138},{x=90.0, y=40.5, z=-71.0},"")

    mat = matreial:new()
    mat.shader = "resources/Shaders/mesh"
    mat.textures = {"resources/Textures/white.png"}
    mat.color = {r=1,g=0,b=1,a=1}
    walker_cube = create_mesh(global_data.layers.cenary,false,path[1],{x=0, y=0, z=0},{x=1, y=1, z=1},2,{mat},{mesh_location:new("resources/3D Models/visual_debug_shapes.gltf","Cube")})
    
end

last_progression = 0

last_progression_2 = 0

function UPDATE()
    time:get()

    if last_progression ~= nil then
        local walk_ret = walk_along_the_path(path,last_progression,1)
        
        if walk_ret == nil then
            last_progression = nil
        else
            mat.color = {r=0,g=1,b=0,a=1}
            create_mesh(global_data.layers.cenary,false,walk_ret.position,{x=0, y=0, z=0},{x=0.2, y=0.2, z=0.2},2,{mat},{mesh_location:new("resources/3D Models/visual_debug_shapes.gltf","Cube")})
            last_progression = walk_ret.progression
        end 
    end

    if last_progression_2 ~= nil then
        local walk_ret = walk_along_the_path(path,last_progression_2,time.delta * 5)
        
        if walk_ret == nil then
            last_progression_2 = nil
        else
            
            local current_pos = deepcopy(walker_cube.components.transform.position)
            walker_cube.components.transform.position = {x=current_pos.x + walk_ret.position_movement.x, y=current_pos.y + walk_ret.position_movement.y, z=current_pos.z + walk_ret.position_movement.z}
            walker_cube.components.transform.rotation = walk_ret.rotation
            walker_cube.components.transform:set()
            print("rotation",walk_ret.rotation.x,walk_ret.rotation.y,walk_ret.rotation.z)

            last_progression_2 = walk_ret.progression
        end 
    end
    
end

function COLLIDE(collision_info)
end

function END()
end
