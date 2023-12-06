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


advanced_ui_style = {}
function advanced_ui_style:new()
    return {
        text_color = {r=1,g=0,b=0,a=1},
        background_color = {r=0,g=1,b=0,a=1},
        border_color = {r=0,g=0,b=1,a=1},
        border_size = 0.05,
        background_image = "",
        border_image = "",
        text_font = "",
    }
end

setmetatable(advanced_ui_style,{
    __call=function (self)
        return self:new()
    end
})

ui_component = create_base_component(components.ui_component)
ui_component.object_ptr = ""
ui_component.type = ""


function ui_component:clean()
    self.type = ""
end
function ui_component:get()
end
function ui_component:set()
end
function ui_component:set_interaction_function(global_func_name)
end
component_map[components.ui_component] = apply_component_metatable(camera_component:new(nil))