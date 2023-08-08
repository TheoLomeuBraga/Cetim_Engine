function get_post_processing_json()
end
function set_post_processing_json(json)
end
function get_set_render_layer_instruction()
end
function get_set_render_layer_instruction(get_set,object)
    
end

render_layer = {}
function render_layer:new()
    ret = {}
    ret.camera_selected = 0
    ret.start_render = true
    ret.clean_color = true
    ret.clean_deep = true
    ret.enable = true
    ret.end_render = true
    ret.use_deep = true
    
    return ret
end

renders_layers = {}
renders_layers.layers_size = 0
renders_layers.layers = {render_layer:new()}
function renders_layers:get()
    renders_layers.layers = deepcopy(get_set_render_layer_instruction(get_lua))
end
function renders_layers:set()
    get_set_render_layer_instruction(get_lua,renders_layers.layers)
end