require("TMP_libs.components.extras")
require("TMP_libs.objects.game_object")
require("TMP_libs.components.transform")
require("TMP_libs.components.render_mesh")
require("TMP_libs.components.physics_3D")
require("TMP_libs.components.physics_2D")
require("TMP_libs.objects.collision_shapes")


function create_collision_3D(father, pos, rot, sca, rigid_boady,shape,cillision_mesh,triger)
    ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    
    ret:add_component(components.physics_3D)
    if rigid_boady then
        ret.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
    else
        ret.components[components.physics_3D].boady_dynamic = boady_dynamics.static
    end
    ret.components[components.physics_3D].collision_shape = shape
    if shape == collision_shapes.convex then
        ret.components[components.physics_3D].collision_mesh = deepcopyjson(cillision_mesh)
    end

    ret.components[components.physics_3D].triger = triger
    ret.components[components.physics_3D].scale = deepcopyjson(sca)
    ret.components[components.physics_3D]:set()
    
    return ret
end

function create_collision_2D(father, pos, rot, sca, rigid_boady,shape,vertex_data,triger)
    ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    ret:add_component(components.physics_2D)

    ret.components[components.physics_2D].scale = {x = sca.x,y = sca.y}
    if rigid_boady then
        ret.components[components.physics_2D].boady_dynamic = boady_dynamics.dynamic
    else
        ret.components[components.physics_2D].boady_dynamic = boady_dynamics.static
    end
    ret.components[components.physics_2D].collision_shape = shape
    if shape == collision_shapes.convex then
        ret.components[components.physics_2D].vertex = deepcopyjson(vertex_data)
    end
    ret.components[components.physics_2D].triger = triger


    ret.components[components.physics_2D]:set()
    
    return ret
end