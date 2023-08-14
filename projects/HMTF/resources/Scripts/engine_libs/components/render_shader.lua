require("engine_libs.components.base_component")
require("engine_libs.components.component_index")
require("engine_libs.objects.material")

function get_set_render_shader(get_set,object)
end


render_shader_component = create_base_component(components.transform)
render_shader_component.layer = 2
render_shader_component.vertex_size = 6
render_shader_component.material = matreial:new()

function render_shader_component:clean()
    self = {}
    self.layer = 2
    self.vertex_size = 6
    self.material = matreial:new()
end
function render_shader_component:get()
    local j = get_set_render_shader(get_lua,self.object_ptr)
    self.layer =  j.layer
    self.material = deepcopyjson(j.material) 
    self.vertex_size = j.vertex_size
end
function render_shader_component:set()
    get_set_render_shader(set_lua,deepcopyjson(self))
end
component_map[components.render_shader] = render_shader_component:new(nil)