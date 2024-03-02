register_function_set("navmesh")

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

register_function_set("debug")

walk_data_meta_table = {
    __call = function(self)
        return {
            path_segment = 0,
            follow_up_progression = 0,
            position = {x=0,y=0,z=0},
        }
    end
}

walk_data = {}

setmetatable(walk_data,walk_data_meta_table)

function walk_along_the_path(speed_or_walk_distance,path,data,use_direction,use_rotation_y,step)
    if use_direction == nil then
        use_direction = false
    end
    if use_rotation_y == nil then
        use_rotation_y = false
    end
    if step == nil then
        step = true
    end
    local ret = {
        position = {x=0,y=0,z=0},
        directional_movement = {x=0,y=0,z=0},
        rotation_y = 0,
    }

    --resto do codigo

    c_print_cube(ret.position)
    return ret
end