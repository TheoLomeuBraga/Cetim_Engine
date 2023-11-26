require("components.extras")
require("objects.game_object")
require("components.transform")
require("components.render_mesh")
require("components.physics_3D")
require("components.physics_2D")
require("objects.collision_shapes")


function create_collision_3D(father, pos, rot, sca, rigid_boady,shape,cillision_mesh,triger)
    ret = game_object(create_object(father))

    ret.components.transform.position = deepcopy(pos)
    ret.components.transform.rotation = deepcopy(rot)
    ret.components.transform.scale = deepcopy({x=1,y=1,z=1})
    ret.components.transform:set()

    
    
    if rigid_boady then
        ret.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
    else
        ret.components[components.physics_3D].boady_dynamic = boady_dynamics.static
    end
    ret.components[components.physics_3D].collision_shape = shape
    if shape == collision_shapes.convex then
        ret.components[components.physics_3D].collision_mesh = deepcopy(cillision_mesh)
    end

    if triger then
        ret.components[components.physics_3D].gravity_scale = 0
        ret.components[components.physics_3D].get_collision_info = true
    else 
        ret.components[components.physics_3D].gravity_scale = 1
    end
    
    ret.components[components.physics_3D].triger = triger
    ret.components[components.physics_3D].scale = (sca)
    ret.components[components.physics_3D]:set()

    --ret.components.transform:change_position(pos.x,pos.y,pos.z)
    --ret.components.transform:change_rotation(rot.x,rot.y,rot.z)
    
    return ret
end

function create_collision_2D(father, pos, rot, sca, rigid_boady,shape,vertex_data,triger)
    ret = game_object(create_object(father))

    
    ret.components.transform.position = deepcopy(pos)
    ret.components.transform.rotation = deepcopy(rot)
    ret.components.transform.scale = deepcopy(sca)
    ret.components.transform:set()

    

    if sca.x == 0 then
        sca.x = 1
    end
    if sca.y == 0 then
        sca.y = 1
    end
    ret.components[components.physics_2D].scale = {x = sca.x,y = sca.y}
    if rigid_boady then
        ret.components[components.physics_2D].boady_dynamic = boady_dynamics.dynamic
    else
        ret.components[components.physics_2D].boady_dynamic = boady_dynamics.static
    end
    ret.components[components.physics_2D].collision_shape = shape
    if shape == collision_shapes.convex then
        ret.components[components.physics_2D].vertex = deepcopy(vertex_data)
    end
    ret.components[components.physics_2D].triger = triger
    ret.components[components.physics_2D]:set()

    --ret.components.transform:change_position(pos.x,pos.y,pos.z)
    --ret.components.transform:change_rotation(rot.x,rot.y,rot.z)
    
    return ret
end