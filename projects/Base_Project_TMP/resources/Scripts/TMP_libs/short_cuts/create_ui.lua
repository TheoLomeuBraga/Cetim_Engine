require("TMP_libs.components.extras")
require("TMP_libs.objects.game_object")
require("TMP_libs.components.transform")
require("TMP_libs.components.render_shader")
require("TMP_libs.short_cuts.create_render_shader")



function create_button(father,is_ui,pos,rot,sca,layer,mat)

    ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].is_ui = is_ui
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    ret:add_component(components.render_shader)
    ret.components[components.render_shader].layer = layer
    ret.components[components.render_shader].material = deepcopy(mat)
    ret.components[components.render_shader]:set()

    return ret

end

function create_text_ui(father,is_ui,pos,rot,sca,layer,mat)

    ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].is_ui = is_ui
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    ret:add_component(components.render_shader)
    ret.components[components.render_shader].layer = layer
    ret.components[components.render_shader].material = deepcopy(mat)
    ret.components[components.render_shader]:set()

    return ret
    
end

function create_input_fild(father,is_ui,pos,rot,sca,layer,mat)

    ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].is_ui = is_ui
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    ret:add_component(components.render_shader)
    ret.components[components.render_shader].layer = layer
    ret.components[components.render_shader].material = deepcopy(mat)
    ret.components[components.render_shader]:set()

    return ret
    
end