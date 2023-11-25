require("components.extras")
require("objects.game_object")
require("components.transform")
require("components.render_mesh")


function create_mesh(father,is_ui,pos,rot,sca,layer,mats,meshes_locations)
    ret = game_object(create_object(father))
    
    ret.components[components.transform].is_ui = is_ui
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    ret.components[components.render_mesh].layer = layer
    ret.components[components.render_mesh].meshes_cout = math.min(tablelength(meshes_locations),tablelength(mats))
    ret.components[components.render_mesh].meshes = deepcopy(meshes_locations)
    ret.components[components.render_mesh].materials = deepcopy(mats)
    ret.components[components.render_mesh]:set()
    return ret
end
