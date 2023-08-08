require("TMP_libs.components.extras")
require("TMP_libs.objects.game_object")
require("TMP_libs.components.transform")
require("TMP_libs.components.render_shader")
require("TMP_libs.short_cuts.create_render_shader")

require("TMP_libs.objects.input")
require("TMP_libs.objects.window")

ui_style = {}
function ui_style:new()
    return {

        color = { r = 1, g = 0, b = 0, a = 1 },
        color_hover = { r = 0, g = 0, b = 1, a = 1 },
        color_click = { r = 0, g = 0, b = 1, a = 1 },

        text_font = "resources/Fonts/Glowworm Regular.json",
        text_color = { r = 0, g = 1, b = 0, a = 1 },

        border_size = 0.5,
        border_roundnes = 0.5,
        border_color = { r = 0, g = 0, b = 1, a = 1 },
        border_color_hover = { r = 0, g = 0, b = 1, a = 1 },
        border_color_click = { r = 0, g = 0, b = 1, a = 1 },

    }
end

local ui_object_example = {

    button_obj = {},
    text_obj = {},

    text = "hello world",

    image = "resources/Textures/white.png",

    click_function = nil,

    style = ui_style:new()

}

function create_ui(father, is_ui, pos, sca, layer, style, text, image, click_function)
    --object
    local core_obj_ptr = create_object(father)
    local button_obj_ptr = create_object(core_obj_ptr)
    local text_obj_ptr = create_object(core_obj_ptr)

    local ret = {
        core_obj = game_object:new(core_obj_ptr),
        button_obj = game_object:new(button_obj_ptr),
        text_obj = game_object:new(text_obj_ptr),
        text = text,
        image = image,
        click_function = click_function,
        style = style,

        hover = false,
        click = false,
    }

    --transform
    ret.core_obj:add_component(components.transform)
    ret.button_obj:add_component(components.transform)
    ret.text_obj:add_component(components.transform)
    function ret:set_transform(is_ui, pos, sca)
        self.core_obj.components[components.transform].is_ui = is_ui
        self.button_obj.components[components.transform].is_ui = is_ui
        self.text_obj.components[components.transform].is_ui = is_ui

        self.core_obj.components[components.transform].position = deepcopy(pos)
        self.core_obj.components[components.transform].scale = deepcopy(sca)

        self.core_obj.components[components.transform]:set()
        self.button_obj.components[components.transform]:set()
        self.text_obj.components[components.transform]:set()
    end

    ret:set_transform(is_ui, pos, sca)

    --style
    ret.button_obj:add_component(components.render_shader)

    function ret:set_style(style)
        local render_shader_mat = matreial:new()
        render_shader_mat.shader = "resources/Shaders/button"
        render_shader_mat.color = deepcopy(style.color)
        render_shader_mat.inputs[3] = style.border_color.r
        render_shader_mat.inputs[4] = style.border_color.g
        render_shader_mat.inputs[5] = style.border_color.b
        render_shader_mat.inputs[6] = style.border_color.a

        render_shader_mat.inputs[1] = style.border_size
        render_shader_mat.inputs[2] = style.border_roundnes

        --colors

        if image == nil or image == "" then
            render_shader_mat.textures[1] = "resources/Textures/white.png"
        else
            render_shader_mat.textures[1] = image
        end

        self.button_obj.components[components.render_shader].material = deepcopy(render_shader_mat)
        self.button_obj.components[components.render_shader]:set()

        --[[
        if text ~= nil and text ~= "" then
            local render_text_mat = matreial:new()
            render_text_mat.shader = "resources/Shaders/text"
        end
        ]]
    end

    ret:set_style(style)

    function ret:UPDATE()
        window:get()
        local mouse_pos = {
            x = keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.normalized_x]),
            y = keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.normalized_y]),
        }

        self.core_obj.components[components.transform]:get()
        local pos  = deepcopy(self.core_obj.components[components.transform].position)
        pos.x      = (pos.x + 1) / 2
        pos.y      = (pos.y + 1) / 2
        local sca  = deepcopy(self.core_obj.components[components.transform].scale)
        sca.x      = sca.x / 2
        sca.y      = sca.y / 2

        self.hover = (mouse_pos.x > pos.x and mouse_pos.x < (pos.x + sca.x)) and
        (mouse_pos.y < pos.y and mouse_pos.y > (pos.y - sca.y))

        self.click = self.hover and
        keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.left]) == 1

        if  not self.hover then
            print("not hover")
            self.button_obj.components[components.render_shader].color = deepcopy(self.style.color)
            self.button_obj.components[components.render_shader].material.inputs[3] = self.style.border_color.r
            self.button_obj.components[components.render_shader].material.inputs[4] = self.style.border_color.g
            self.button_obj.components[components.render_shader].material.inputs[5] = self.style.border_color.b
            self.button_obj.components[components.render_shader].material.inputs[6] = self.style.border_color.a
        elseif  self.hover and not self.click then
            print("hover")
            self.button_obj.components[components.render_shader].color = deepcopy(self.style.color_hover)
            self.button_obj.components[components.render_shader].material.inputs[3] = self.style.border_color_hover.r
            self.button_obj.components[components.render_shader].material.inputs[4] = self.style.border_color_hover.g
            self.button_obj.components[components.render_shader].material.inputs[5] = self.style.border_color_hover.b
            self.button_obj.components[components.render_shader].material.inputs[6] = self.style.border_color_hover.a
            deepprint(self.style.color_hover)
        elseif self.click then
            print("click")
            self.button_obj.components[components.render_shader].color = deepcopy(self.style.color_click)
            self.button_obj.components[components.render_shader].material.inputs[3] = self.style.border_color_click.r
            self.button_obj.components[components.render_shader].material.inputs[4] = self.style.border_color_click.g
            self.button_obj.components[components.render_shader].material.inputs[5] = self.style.border_color_click.b
            self.button_obj.components[components.render_shader].material.inputs[6] = self.style.border_color_click.a
        end
        self.button_obj.components[components.render_shader]:set()

        if self.click and click_function ~= nil then
            self.click_function()
        end
    end

    return ret
end
