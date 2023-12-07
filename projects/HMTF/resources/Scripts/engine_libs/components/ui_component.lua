register_function_set("render")

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

render_text_location = {
    none = 0,
    center = 1,
    top = 2,
    down = 3,
    left = 4,
    right = 5
}

advanced_ui_style = {}
function advanced_ui_style:new()
    return {
        text_color = { r = 1, g = 0, b = 0, a = 1 },
        background_color = { r = 0, g = 1, b = 0, a = 1 },
        border_color = { r = 0, g = 0, b = 1, a = 1 },
        border_size = 0.05,
        background_image = "",
        border_image = "",
        text_font = "",
    }
end

setmetatable(advanced_ui_style, {
    __call = function(self)
        return self:new()
    end
})

ui_component = create_base_component(components.ui_component)
ui_component.object_ptr = ""
ui_component.type = ""
ui_component.layer = 5
ui_component.id = ""
ui_component.text = ""
ui_component.text_size = 0.1
ui_component.uniform_spaces_betwen_chars = false
ui_component.text_location_x = render_text_location.center
ui_component.text_location_y = render_text_location.center
ui_component.positiom { x = 0.5, y = 0.5 }
ui_component.scale = { x = 0.2, y = 0.2 }
ui_component.normal_style = advanced_ui_style()
ui_component.hover_style = advanced_ui_style()
ui_component.click_style = advanced_ui_style()
ui_component.text = ""
ui_component.state = ""


function ui_component:clean()
    self.type = ""
    self.layer = 5
    self.id = ""
    self.text = ""
    self.text_size = 0.1
    self.uniform_spaces_betwen_chars = false
    self.text_location_x = render_text_location.center
    self.text_location_y = render_text_location.center
    self.positiom { x = 0.5, y = 0.5 }
    self.scale = { x = 0.2, y = 0.2 }
    self.normal_style = advanced_ui_style()
    self.hover_style = advanced_ui_style()
    self.click_style = advanced_ui_style()
    self.text = ""
    self.state = ""
end

function ui_component:get()
end

function ui_component:set()
end

function ui_component:set_interaction_function(global_func_name)
end

component_map[components.ui_component] = apply_component_metatable(camera_component:new(nil))
