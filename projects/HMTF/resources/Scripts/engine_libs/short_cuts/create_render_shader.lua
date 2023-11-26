require("components.base_component")
require("components.extras")
require("objects.game_object")
require("components.transform")
require("components.render_shader")

function create_render_shader(father,is_ui,pos,rot,sca,layer,mat)
    ret = game_object(create_object(father))
    ret.components.transform.is_ui = is_ui
    ret.components.transform.position = deepcopy(pos)
    ret.components.transform.rotation = deepcopy(rot)
    ret.components.transform.scale = deepcopy(sca)
    ret.components.transform:set()

    ret.components[components.render_shader].layer = layer
    ret.components[components.render_shader].material = deepcopy(mat)
    ret.components[components.render_shader]:set()
    return ret
end