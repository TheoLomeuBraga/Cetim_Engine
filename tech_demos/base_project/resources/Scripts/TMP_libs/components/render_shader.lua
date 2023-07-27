require("TMP_libs.components.component_index")
require("TMP_libs.objects.material")

function get_set_render_shader(get_set,object)
end


render_shader_component = {}
function render_shader_component:new(object_ptr)
    rs = {}
    rs.object_ptr = object_ptr
    rs.layer = 1
    rs.vertex_size = 6
    rs.material = matreial:new()
    function rs:get()
        j = get_set_render_shader(get_lua,self.object_ptr)
        self.layer =  j.layer
        self.material = deepcopyjson(j.material) 
        self.vertex_size = j.vertex_size
    end
    function rs:set()
        get_set_render_shader(set_lua,deepcopyjson(self))
    end
    function rs:delet()
        
    end
    return rs
end
component_map[components.render_shader] = render_shader_component