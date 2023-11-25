require("components.extras")
require("objects.game_object")
require("components.transform")
require("components.render_text")

function create_text(father,is_ui,pos,rot,sca,material,layer,text,font)

    ret = game_object(create_object(father))
    ret.components[components.transform].is_ui = is_ui
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()
    
    ret.components[components.render_text].layer = layer
    ret.components[components.render_text].material = deepcopy(material)
    ret.components[components.render_text].font = font
    ret.components[components.render_text].text = text
    ret.components[components.render_text]:set()
    
    return ret
end