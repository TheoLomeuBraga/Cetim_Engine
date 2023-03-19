require("TMP_libs.components.component_index")

--text

function get_text_json(object)
end
function set_text_json(object,json)
end

function get_set_render_text(get_set,object)
end

render_text_component = {}
function render_text_component:new(object_ptr)
    rs = {}
    rs.object_ptr = object_ptr
    rs.layer = 1
    rs.font = ""
    rs.text = ""
    rs.line_size = 34
    rs.space_betwen_chars = 2
    rs.max_space_betwen_chars = 3
    rs.min_space_betwen_chars = 0
    rs.material = matreial:new()
    function rs:get()
        j = get_set_render_text(get_lua,self.object_ptr)

        self.layer = j.layer
        self.font = j.font
        self.text = j.text
        self.line_size = j.line_size
        self.space_betwen_chars = j.space_betwen_chars
        self.max_space_betwen_chars = j.max_space_betwen_chars
        self.min_space_betwen_chars = j.min_space_betwen_chars
        self.material = j.material

        
    end
    function rs:set()
        get_set_render_text(set_lua,deepcopyjson(self))
    end
    function rs:delet()
    end
    return rs
end
component_map[components.render_text] = render_text_component