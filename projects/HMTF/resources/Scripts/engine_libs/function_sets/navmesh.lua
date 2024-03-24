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
    if tag == nil then
        return c_generate_navmesh_path(start,goal,"")
    end
    return c_generate_navmesh_path(start,goal,tag)
end

function look_to(start,goal,rot_x,rot_y,rot_z)
    return c_look_to(start,goal,rot_x,rot_y,rot_z)
end

function walk_along_the_path(path,progression,speed_or_walk_distance)
    return c_walk_along_the_path(path,progression,speed_or_walk_distance)
end

require("components.extras")
require("objects.game_object")
require("components.transform")
require("objects.time")
require("components.component_all")

function walk_to(obj,path,last_progression, hight, speed)

    if last_progression ~= nil then
        
        local walk_ret = walk_along_the_path(path, last_progression_2, time.scale * time.delta * speed)

        if walk_ret == nil then
            last_progression_2 = nil
        else
            local current_pos = deepcopy(obj.components.transform.position)
            obj.components.transform.position = walk_ret.position
            walk_ret.position.y = walk_ret.position.y + (hight / 2)

            if walk_ret.rotation ~= nil then
                obj.components.transform.rotation = walk_ret.rotation
            end
            obj.components.transform:set()

            last_progression_2 = walk_ret.progression
        end
    end
    
end
