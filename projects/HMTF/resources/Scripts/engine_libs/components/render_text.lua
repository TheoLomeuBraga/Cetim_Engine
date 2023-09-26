register_function_set("render")

require("engine_libs.components.base_component")
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

render_text_component = create_base_component(components.render_text)
render_text_component.layer = 2
render_text_component.font = ""
render_text_component.text = ""
render_text_component.line_size = 1000
render_text_component.uniform_space_between_characters = false
render_text_component.material = matreial:new()
render_text_component.text_location_x = render_text_location.center
render_text_component.text_location_y = render_text_location.center
function render_text_component:clean()
    self.layer = 2
    self.font = ""
    self.text = ""
    self.line_size = 1000
    self.uniform_space_between_characters = false
    self.material = matreial:new()
    self.text_location_x = render_text_location.center
    self.text_location_y = render_text_location.center
end
    function render_text_component:get()
        local j = get_set_render_text(get_lua,self.object_ptr)
        self.layer = j.layer
        self.font = j.font
        self.text = j.text
        self.line_size = j.line_size
        self.uniform_space_between_characters = j.uniform_space_between_characters
        self.material = j.material
        self.text_location_x = j.text_location_x
        self.text_location_x = j.text_location_y

        
    end
    function render_text_component:set()
        get_set_render_text(set_lua,deepcopyjson(self))
    end
    function render_text_component:get_text_size()
        return get_text_size(self.object_ptr)
    end
component_map[components.render_text] = render_text_component:new(nil)