require("components.extras")
require("objects.game_object")
require("components.transform")
require("components.render_text")

function create_text(father,is_ui,pos,rot,sca,material,layer,text,font)

    ret = game_object(create_object(father))
    ret.components.transform.is_ui = is_ui
    ret.components.transform.position = deepcopy(pos)
    ret.components.transform.rotation = deepcopy(rot)
    ret.components.transform.scale = deepcopy(sca)
    ret.components.transform:set()
    
    ret.components.render_text.layer = layer
    ret.components.render_text.material = deepcopy(material)
    ret.components.render_text.font = font
    ret.components.render_text.text = text
    ret.components.render_text:set()
    
    return ret
end