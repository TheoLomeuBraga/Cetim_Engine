register_function_set("debug")

require("engine_libs.function_sets.navmesh")
require("engine_libs.short_cuts.create_mesh")
require("components.component_all")
require("objects.time")



this_object = nil

path = {}

function START()
    

    path = generate_navmesh_path({x=-21, y=40.5, z=-138},{x=90.0, y=40.5, z=-71.0},"")
    
    --[[
    for key, value in pairs(path) do
        c_print_cube(value)
    end
    ]]
end

last_progression = 0
function UPDATE()
    time:get()
    if last_progression ~= nil then
        local walk_ret = walk_along_the_path(path,last_progression,1)
        
        if walk_ret == nil then
            last_progression = nil
        else
            c_print_cube(walk_ret.target)
            last_progression = walk_ret.progression
        end 
    end
end

function COLLIDE(collision_info)
end

function END()
end
