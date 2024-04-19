register_function_set("render")
register_function_set("input")

require("components.base_component")
require("components.component_index")

--[[
struct ui_style_struct
{
    vec4 color = vec4(1, 0, 0, 1);
    vec4 background_color = vec4(0, 1, 0, 1);
    vec4 border_color = vec4(0, 0, 1, 1);

    float border_size = 0.05;

    shared_ptr<imagem> background_image = NULL;
    shared_ptr<imagem> border_image = NULL;

    shared_ptr<fonte> text_font = NULL;
};
]]

function set_ui_curson_location(v2,click)
    c_set_ui_curson_location(v2,click)
end


function set_ui_selection_id(id,click)
    c_set_ui_selection_id(id,click)
end

render_text_location = {
    none = 0,
    center = 1,
    top = 2,
    down = 3,
    left = 4,
    right = 5
}

ui_types = {
    common = 0,
    check_box = 1,
    slider = 2,
}

advanced_ui_style = {}
function advanced_ui_style:new()
    return {
        text_color = { r = 1, g = 0, b = 0, a = 1 },
        background_color = { r = 0, g = 1, b = 0, a = 1 },
        border_color = { r = 0, g = 0, b = 1, a = 1 },
        border_size = 0.05,
        text_size = 0.1,
        uniform_space_between_characters = 0,
        space_betwen_lines = 2,
        background_image = "Textures/null.svg",
        border_image = "Textures/null.svg",
        text_font = "Fonts/OpenSans.ttf",
    }
end

setmetatable(advanced_ui_style, {
    __call = function(self)
        return self:new()
    end
})

empty_style = advanced_ui_style:new()
empty_style.text_color = { r = 0, g = 0, b = 0, a = 0 }
empty_style.background_color = { r = 0, g = 0, b = 0, a = 0 }
empty_style.border_color = { r = 0, g = 0, b = 0, a = 0 }
empty_style.border_size = 0
empty_style.text_size = 0
empty_style.background_image = "Textures/white.png"
empty_style.border_image = "Textures/white.png"

ui_component = create_base_component(components.ui_component)
ui_component.object_ptr = ""
ui_component.type = ""
ui_component.layer = 5
ui_component.ui_type = 0
ui_component.selection_id = 0
ui_component.id = ""
ui_component.data = ""
ui_component.text = ""
ui_component.text_location_x = render_text_location.center
ui_component.text_location_y = render_text_location.center
ui_component.position = { x = 0.5, y = 0.5 }
ui_component.scale = { x = 0.2, y = 0.2 }
ui_component.normal_style = advanced_ui_style()
ui_component.hover_style = advanced_ui_style()
ui_component.click_style = advanced_ui_style()
ui_component.text = ""
ui_component.state = ""


function ui_component:clean()
    self.type = ""
    self.layer = 5
    self.ui_type = 0
    self.selection_id = 0
    self.id = ""
    self.data = ""
    self.text = ""
    self.uniform_spaces_betwen_chars = false
    self.text_location_x = render_text_location.center
    self.text_location_y = render_text_location.center
    self.position = { x = 0.5, y = 0.5 }
    self.scale = { x = 0.2, y = 0.2 }
    self.normal_style = advanced_ui_style()
    self.hover_style = advanced_ui_style()
    self.click_style = advanced_ui_style()
    self.text = ""
    self.state = ""
end

function ui_component:get()
    local u = get_set_ui_component(get_lua, self.object_ptr)
    self.type = u.type
    self.layer = u.layer
    self.ui_type = u.ui_type
    self.selection_id = u.selection_id
    self.id = u.id
    self.data = u.data
    self.text = u.text
    self.text_location_x = u.text_location_x
    self.text_location_y = u.text_location_y
    self.position = u.position
    self.scale = u.scale
    self.normal_style = u.normal_style
    self.hover_style = u.hover_style
    self.click_style = u.click_style
    self.text = u.text
    self.state = u.state
end

function ui_component:set()
    get_set_ui_component(set_lua, deepcopyjson(self))
end

function ui_component:set_interaction_function(global_func_name)
    set_ui_component_function(self.object_ptr, global_func_name)
end

component_map[components.ui_component] = apply_component_metatable(ui_component:new(nil))
