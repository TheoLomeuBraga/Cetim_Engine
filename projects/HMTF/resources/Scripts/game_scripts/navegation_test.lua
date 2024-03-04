require("engine_libs.function_sets.navmesh")
require("engine_libs.short_cuts.create_mesh")

this_object = nil

function START()

    this_object = game_object(this_object_ptr)

    create_mesh(this_object_ptr,false,{x=0,y=0,z=0},{x=0,y=0,z=0},{x=1,y=1,z=1},2,nil,nil)
    
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end
