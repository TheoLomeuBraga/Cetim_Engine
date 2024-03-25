register_function_set("render")

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
renders_layers.layers = {render_layer:new()}
function renders_layers:get()
    renders_layers.layers = (get_set_render_layer_instruction(get_lua))
end
function renders_layers:set()
    get_set_render_layer_instruction(set_lua,renders_layers.layers)
end