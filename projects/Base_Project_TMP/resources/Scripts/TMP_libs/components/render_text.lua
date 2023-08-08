require("TMP_libs.components.component_index")
require("TMP_libs.objects.material")

--text

function get_text_json(object)
end
function set_text_json(object,json)
end

function get_set_render_text(get_set,object)
end

function get_text_size(object)
end

render_text_component = {}
function render_text_component:new(object_ptr)
    local rs = {}
    rs.object_ptr = object_ptr
    rs.layer = 2
    rs.font = ""
    rs.text = ""
    rs.line_size = 34
    rs.uniform_space_between_characters = false
    rs.material = matreial:new()
    function rs:get()
        j = get_set_render_text(get_lua,self.object_ptr)

        self.layer = j.layer
        self.font = j.font
        self.text = j.text
        self.line_size = j.line_size
        self.uniform_space_between_characters = j.uniform_space_between_characters
        self.material = j.material

        
    end
    function rs:set()
        get_set_render_text(set_lua,deepcopyjson(self))
    end
    function rs:get_text_size()
        return get_text_size(self.object_ptr)
    end
    function rs:delet()
    end
    return rs
end
component_map[components.render_text] = render_text_component