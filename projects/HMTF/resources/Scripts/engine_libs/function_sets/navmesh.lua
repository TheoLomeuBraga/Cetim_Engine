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
require("components.component_all")

function walk_to(obj,path,progression_ptr, hight, step_size,look_up)

    if look_up == nil then
        look_up = false
    end

    if progression_ptr[1] ~= nil then
        
        local walk_ret = walk_along_the_path(path, progression_ptr[1],  step_size)

        if walk_ret == nil then
            progression_ptr[1] = nil
        else

            
            local current_pos = walk_ret.position
            current_pos.y = walk_ret.position.y + (hight / 2)
            obj.components.transform:change_position(current_pos.x,current_pos.y,current_pos.z)

            --rotation

            local rot = {x=0,y=0,z=0}
            if walk_ret.rotation ~= nil then
                if look_up then
                    rot = walk_ret.rotation
                else
                    rot.y = walk_ret.rotation.y
                end
                
            end

            obj.components.transform:change_rotation(rot.x,rot.y,rot.z)
            

            progression_ptr[1] = walk_ret.progression
        end
    end
    
end
