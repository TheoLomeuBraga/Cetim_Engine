register_function_set("debug")

require("engine_libs.function_sets.navmesh")
require("engine_libs.short_cuts.create_mesh")
require("components.component_all")



this_object = nil

path = {}

function START()
    

    path = generate_navmesh_path({x=-21, y=40.5, z=-138},{x=90.0, y=40.5, z=-71.0},"")
    
    for key, value in pairs(path) do
        c_print_cube(value)
    end
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end
