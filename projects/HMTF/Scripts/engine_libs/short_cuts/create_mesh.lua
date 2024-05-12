require("components.extras")
require("objects.game_object")
require("components.transform")
require("components.render_mesh")


function create_mesh(father,is_ui,pos,rot,sca,layer,mats,meshes_locations,use_oclusion)
    if use_oclusion == nil then
        use_oclusion = true
    end

    ret = game_object(create_object(father))
    
    ret.components.transform.is_ui = is_ui
    ret.components.transform.position = deepcopy(pos)
    ret.components.transform.rotation = deepcopy(rot)
    ret.components.transform.scale = deepcopy(sca)
    ret.components.transform:set()

    ret.components.render_mesh.layer = layer
    ret.components.render_mesh.meshes_cout = math.min(tablelength(meshes_locations),tablelength(mats))
    ret.components.render_mesh.meshes = deepcopy(meshes_locations)
    ret.components.render_mesh.materials = deepcopy(mats)
    ret.components.render_mesh.use_oclusion = use_oclusion
    ret.components.render_mesh:set()
    return ret
end
