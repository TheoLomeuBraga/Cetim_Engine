require("TMP_libs.objects.game_object")
require("TMP_libs.components.transform")
require("TMP_libs.components.render_mesh")
require("TMP_libs.components.physics_3D")


function create_collision_mesh(father,pos,rot,sca,render_layer,mats,meshes_locations,rigid_boady)
    ret = game_object:new(create_object(father))
    
    ret:add_component(components.transform)
    ret.components[components.transform].is_ui = is_ui
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    ret:add_component(components.render_mesh)
    ret.components[components.render_mesh].layer = render_layer
    ret.components[components.render_mesh].meshes_cout = math.min(tablelength(meshes_locations),tablelength(mats))
    ret.components[components.render_mesh].meshes = deepcopy(meshes_locations)
    ret.components[components.render_mesh].materials = deepcopy(mats)
    ret.components[components.render_mesh]:set()
    return ret
end
