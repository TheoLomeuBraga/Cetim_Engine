require("engine_libs.components.component_index")
require("engine_libs.objects.material")

--text

render_text_location = {
    none = 0,
	center = 1,
	top = 2,
	down = 3,
	left = 4,
	right = 5
}

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
    rs.line_size = 1000
    rs.uniform_space_between_characters = false
    rs.material = matreial:new()
    rs.text_location_x = render_text_location.center
    rs.text_location_y = render_text_location.center
    function rs:get()
        j = get_set_render_text(get_lua,self.object_ptr)

        self.layer = j.layer
        self.font = j.font
        self.text = j.text
        self.line_size = j.line_size
        self.uniform_space_between_characters = j.uniform_space_between_characters
        self.material = j.material
        self.text_location_x = j.text_location_x
        self.text_location_x = j.text_location_y

        
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