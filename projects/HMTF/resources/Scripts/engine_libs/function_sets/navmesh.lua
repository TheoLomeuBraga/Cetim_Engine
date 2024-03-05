register_function_set("navmesh")
require("math")

function show_navmesh(on)
    c_show_navmesh(on)
end

function remove_navmesh(tag)
    c_remove_navmesh(tag)
end

function remove_all_navmesh()
    c_remove_all_navmesh()
end

function create_navmesh(position,rotation,scale,mesh,tag)
    c_create_navmesh(position,rotation,scale,mesh,tag)
end

function generate_navmesh_path(start,goal,tag)
    return c_generate_navmesh_path(start,goal,tag)
end

function look_to(start,goal,rot_x,rot_y,rot_z)
    return c_look_to(start,goal,rot_x,rot_y,rot_z)
end

--[[
local walk_along_the_path_ret_example = {
    position = {x=0,y=0,z=0},
    position_movement = {x=0,y=0,z=0},
    rotation = {x=0,y=0,z=0},
    progression = 0
}
]]
function walk_along_the_path(path,progression,speed_or_walk_distance)
    return c_walk_along_the_path(path,progression,speed_or_walk_distance)
end


