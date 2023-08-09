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

ui_category = {
    display = 0,
    button = 1,
    input_fild = 2,
}

local ui_object_example = {

    button_obj = {},
    text_obj = {},

    text = "hello world",

    image = "resources/Textures/white.png",

    click_function = nil,

    style = ui_style:new()

}



function create_ui(father, is_ui, pos, sca, layer, style, text,text_size, image, click_function,category)
    --object
    local core_obj_ptr = create_object(father)
    
    local button_obj_ptr = create_object(core_obj_ptr)
    local text_obj_ptr = create_object(core_obj_ptr)

    --[[
        ret.components[components.render_text].layer = layer
        ret.components[components.render_text].material = deepcopy(material)
        ret.components[components.render_text].font = font
        ret.components[components.render_text].text = text
    ]]
    

    local ret = {
        core_obj = game_object:new(core_obj_ptr),
        button_obj = game_object:new(button_obj_ptr),
        text_obj = game_object:new(text_obj_ptr),
        text = text,
        image = image,
        click_function = click_function,
        style = style,
        hover = false,
        category = category,


        click_last_frame = false,
        click = false,
        insertion_mode = false,
        insertion_mode_last_frame = false,
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

        self.text_obj.components[components.transform].position = {x=0.5,y=-0.5,z=0}
        self.text_obj.components[components.transform].scale = {x=text_size,y=text_size,z=text_size}

        self.core_obj.components[components.transform]:set()
        self.button_obj.components[components.transform]:set()
        self.text_obj.components[components.transform]:set()
    end

    ret:set_transform(is_ui, pos, sca)

    --style
    ret.button_obj:add_component(components.render_shader)
    ret.text_obj:add_component(components.render_text)
    
    ret.text_obj.components[components.render_text].text = text
    ret.text_obj.components[components.render_text].layer = layer
    --ret.text_obj.components[components.render_text].line_size =  ret.core_obj.components[components.transform].scale.x / text_size
    ret.text_obj.components[components.render_text]:set()

    ret.button_obj.components[components.render_shader].layer = layer
    ret.button_obj.components[components.render_shader]:set()

    ret.button_obj.components[components.render_shader]:set()

    function ret:set_style(style)
        self.style = deepcopy(self.style)

        local render_shader_mat = matreial:new()
        render_shader_mat.shader = "resources/Shaders/button"
        render_shader_mat.color = deepcopy(self.style.color)
        render_shader_mat.inputs[3] = self.style.border_color.r
        render_shader_mat.inputs[4] = self.style.border_color.g
        render_shader_mat.inputs[5] = self.style.border_color.b
        render_shader_mat.inputs[6] = self.style.border_color.a

        render_shader_mat.inputs[1] = self.style.border_size
        render_shader_mat.inputs[2] = self.style.border_roundnes

        --colors

        if image == nil or image == "" then
            render_shader_mat.textures[1] = "resources/Textures/white.png"
        else
            render_shader_mat.textures[1] = image
        end

        self.button_obj.components[components.render_shader].material = deepcopy(render_shader_mat)
        self.button_obj.components[components.render_shader]:set()

        
        if text ~= nil and text ~= "" then
            local render_text_mat = matreial:new()
            render_text_mat.shader = "resources/Shaders/text"
            render_text_mat.color = deepcopy(self.style.text_color)

            
            self.text_obj.components[components.render_text].material = deepcopy(render_text_mat)
            self.text_obj.components[components.render_text].font = self.style.text_font
            

            self.text_obj.components[components.render_text]:set()
        end
        
    end

    ret:set_style(style)

    function ret:set_text(new_text)
        self.text_obj.components[components.render_text].text = new_text
        self.text_obj.components[components.render_text]:set()
        self.text = new_text
    end

    function ret:UPDATE()
        window:get()

        if self.category ~= 0 then
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
                self.button_obj.components[components.render_shader].material.color = deepcopy(self.style.color)
                self.button_obj.components[components.render_shader].material.inputs[3] = self.style.border_color.r
                self.button_obj.components[components.render_shader].material.inputs[4] = self.style.border_color.g
                self.button_obj.components[components.render_shader].material.inputs[5] = self.style.border_color.b
                self.button_obj.components[components.render_shader].material.inputs[6] = self.style.border_color.a
            elseif  self.hover and not self.click then
                self.button_obj.components[components.render_shader].material.color = deepcopy(self.style.color_hover)
                self.button_obj.components[components.render_shader].material.inputs[3] = self.style.border_color_hover.r
                self.button_obj.components[components.render_shader].material.inputs[4] = self.style.border_color_hover.g
                self.button_obj.components[components.render_shader].material.inputs[5] = self.style.border_color_hover.b
                self.button_obj.components[components.render_shader].material.inputs[6] = self.style.border_color_hover.a
            elseif self.click then
                self.button_obj.components[components.render_shader].material.color = deepcopy(self.style.color_click)
                self.button_obj.components[components.render_shader].material.inputs[3] = self.style.border_color_click.r
                self.button_obj.components[components.render_shader].material.inputs[4] = self.style.border_color_click.g
                self.button_obj.components[components.render_shader].material.inputs[5] = self.style.border_color_click.b
                self.button_obj.components[components.render_shader].material.inputs[6] = self.style.border_color_click.a
            end
            self.button_obj.components[components.render_shader]:set()
        end
        

        if self.category == 1 then
            if self.click and not self.click_last_frame and click_function ~= nil then
                self.click_function()
            end
            self.click_last_frame = self.click
        end

        if self.category == 2 then
            if self.click and not self.click_last_frame and click_function ~= nil then
                keys_axis:set_text_input_geter(not self.insertion_mode)
                self.insertion_mode = not self.insertion_mode
                
                
                if keys_axis:get_text_input() ~= "" then
                    self.click_function(keys_axis:get_text_input())
                end
                
            end

            if self.insertion_mode then
                self.text = keys_axis:get_text_input()
            end
            

            self.text_obj.components[components.render_text].text = self.text
            self.text_obj.components[components.render_text]:set()

            self.insertion_mode_last_frame = self.insertion_mode
            self.click_last_frame = self.click
        end

        
        

        
    end

    return ret
end
