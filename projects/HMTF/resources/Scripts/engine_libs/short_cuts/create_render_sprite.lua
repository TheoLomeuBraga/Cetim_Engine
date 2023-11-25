require("components.component_table")
require("objects.game_object")
require("components.transform")
require("components.render_sprite")

function create_sprite(father,is_ui,pos,rot,sca,mat,layer,sprite_id,tileset_local)
    ret = game_object(create_object(father))

    
    ret.components[components.transform].is_ui = is_ui
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    
    
    ret.components[components.render_sprite].material = deepcopy(mat)
    ret.components[components.render_sprite].layer = layer
    ret.components[components.render_sprite].selected_tile = sprite_id
    ret.components[components.render_sprite].tile_set_local = tileset_local
    ret.components[components.render_sprite]:set()
    
    
    return ret
end