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

        border_size = 0.1,
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
    progrecive_text_fild = 3,
}

local ui_object_example = {

    button_obj = {},
    text_obj = {},

    text = "hello world",

    image = "resources/Textures/white.png",

    click_function = nil,

    style = ui_style:new()

}



function create_ui(father, pos, sca, layer, style, text,text_size, image, click_function,category)
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
    function ret:set_transform( pos, sca)
        self.core_obj.components[components.transform].is_ui = true
        self.button_obj.components[components.transform].is_ui = true
        self.text_obj.components[components.transform].is_ui = true

        self.button_obj.components[components.transform].position = deepcopy(pos)
        self.button_obj.components[components.transform].scale = deepcopy(sca)

        if category == 3 then
            self.text_obj.components[components.transform].position = {x= pos.x+(sca.x * style.border_size / 2),y=pos.y + (sca.y * style.border_size / 2),z=0}
        else
            self.text_obj.components[components.transform].position = {x= (sca.x / 2) + pos.x,y=-(sca.y / 2) + pos.y - (text_size / 2),z=0}
        end
        
        self.text_obj.components[components.transform].scale = {x=text_size,y=text_size,z=text_size}

        self.core_obj.components[components.transform]:set()
        self.button_obj.components[components.transform]:set()
        self.text_obj.components[components.transform]:set()
    end

    ret:set_transform( pos, sca)

    --style
    ret.button_obj:add_component(components.render_shader)
    ret.text_obj:add_component(components.render_text)

    if category == 3 then
        ret.text_obj.components[components.render_text].text_location_x = render_text_location.right
        ret.text_obj.components[components.render_text].text_location_y = render_text_location.down
    else
        ret.text_obj.components[components.render_text].text_location_x = render_text_location.center
        ret.text_obj.components[components.render_text].text_location_y = render_text_location.center
    end
    
    ret.text_obj.components[components.render_text].text = text
    ret.text_obj.components[components.render_text].layer = layer
    ret.text_obj.components[components.render_text]:set()

    ret.button_obj.components[components.render_shader].layer = layer
    ret.button_obj.components[components.render_shader]:set()

    ret.button_obj.components[components.render_shader]:set()

    function ret:set_style(style)
        self.style = deepcopy(style)

        local render_shader_mat = matreial:new()
        render_shader_mat.shader = "resources/Shaders/button"
        render_shader_mat.color = deepcopy(self.style.color)
        render_shader_mat.inputs[2] = self.style.border_color.r
        render_shader_mat.inputs[3] = self.style.border_color.g
        render_shader_mat.inputs[4] = self.style.border_color.b
        render_shader_mat.inputs[5] = self.style.border_color.a

        render_shader_mat.inputs[1] = self.style.border_size

        --colors

        if image == nil or image == "" then
            render_shader_mat.textures[1] = "resources/Textures/white.png"
        else
            render_shader_mat.textures[1] = image
        end

        self.button_obj.components[components.render_shader].material = deepcopy(render_shader_mat)
        self.button_obj.components[components.render_shader]:set()

        self.text_obj.components[components.render_text].font = self.style.text_font
        
        if text ~= nil and text ~= "" then
            local render_text_mat = matreial:new()
            render_text_mat.shader = "resources/Shaders/text"
            render_text_mat.color = deepcopy(self.style.text_color)

            
            self.text_obj.components[components.render_text].material = deepcopy(render_text_mat)
            
            

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
    
            self.button_obj.components[components.transform]:get()
            local pos  = deepcopy(self.button_obj.components[components.transform].position)
            pos.x      = (pos.x + 1) / 2
            pos.y      = 1 - ((pos.y + 1) / 2) 
            local sca  = deepcopy(self.button_obj.components[components.transform].scale)
            sca.x      = sca.x / 2
            sca.y      = sca.y / 2
    
            self.hover = (mouse_pos.x > pos.x and mouse_pos.x < (pos.x + sca.x)) and (mouse_pos.y > pos.y and mouse_pos.y < (pos.y + sca.y))
    
            self.click = keys_axis:get_input(input_devices.mouse, input_keys.mouse[input_keys.mouse.left]) == 1
    
            if  not self.hover then
                self.button_obj.components[components.render_shader].material.color = deepcopy(self.style.color)
                self.button_obj.components[components.render_shader].material.inputs[2] = self.style.border_color.r
                self.button_obj.components[components.render_shader].material.inputs[3] = self.style.border_color.g
                self.button_obj.components[components.render_shader].material.inputs[4] = self.style.border_color.b
                self.button_obj.components[components.render_shader].material.inputs[5] = self.style.border_color.a
            elseif  self.hover and not self.click then
                self.button_obj.components[components.render_shader].material.color = deepcopy(self.style.color_hover)
                self.button_obj.components[components.render_shader].material.inputs[2] = self.style.border_color_hover.r
                self.button_obj.components[components.render_shader].material.inputs[3] = self.style.border_color_hover.g
                self.button_obj.components[components.render_shader].material.inputs[4] = self.style.border_color_hover.b
                self.button_obj.components[components.render_shader].material.inputs[5] = self.style.border_color_hover.a
            elseif self.hover and  self.click then
                self.button_obj.components[components.render_shader].material.color = deepcopy(self.style.color_click)
                self.button_obj.components[components.render_shader].material.inputs[2] = self.style.border_color_click.r
                self.button_obj.components[components.render_shader].material.inputs[3] = self.style.border_color_click.g
                self.button_obj.components[components.render_shader].material.inputs[4] = self.style.border_color_click.b
                self.button_obj.components[components.render_shader].material.inputs[5] = self.style.border_color_click.a
            end
            self.button_obj.components[components.render_shader]:set()
        else
            self.text_obj.components[components.render_text].text = self.text
            self.text_obj.components[components.render_text]:set()
        end
        

        if self.category == 1 or self.category == 3 then
            if self.hover and  self.click and not self.click_last_frame and click_function ~= nil then
                self.click_function()
            end
            self.click_last_frame = self.click

            self.text_obj.components[components.render_text].text = self.text
            self.text_obj.components[components.render_text]:set()
        end

        if self.category == 2 then
            
            if self.hover and self.click and not self.click_last_frame and click_function ~= nil then
                
                self.insertion_mode = not self.insertion_mode
                keys_axis:set_text_input_geter(self.insertion_mode)
                
                if keys_axis:get_text_input() ~= "" and keys_axis:get_text_input() ~= "" then
                    self.click_function(keys_axis:get_text_input())
                end
                
            end

            if (self.insertion_mode and not self.hover and self.click) or keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.enter]) == 1 then

                if keys_axis:get_text_input() ~= "" and keys_axis:get_text_input() ~= "" then
                    self.click_function(keys_axis:get_text_input())
                end

                self.insertion_mode = false
                keys_axis:set_text_input_geter(self.insertion_mode)
                
            end

            local keyboard_text = ""

            if self.insertion_mode then
                keyboard_text = keys_axis:get_text_input()
            end

            
            
            if keyboard_text ~= "" then
                self.text_obj.components[components.render_text].text = keyboard_text
            else
                self.text_obj.components[components.render_text].text = self.text
            end
            
            self.text_obj.components[components.render_text]:set()

            self.insertion_mode_last_frame = self.insertion_mode
            self.click_last_frame = self.click
        end

        
        

        
    end

    function ret:END()
        if self.core_obj.object_ptr ~= nil then
            remove_object(self.core_obj.object_ptr)
            self.core_obj.object_ptr = nil
        end
    end

    return ret
end

ui_element_for_matrix = {}
function ui_element_for_matrix:new()
    return {
        --pos = {x=1,y=1}, 
        sca = {x=1,y=1}, 
        style = ui_style:new(),
        text = "",
        text_size = 0.05, 
        image = "resources/Textures/white.png",
        click_function = nil,
        category = ui_category.display
    }
end

function ui_element_matrix_example()
    local a = ui_element_for_matrix:new()
    a.text = "a"

    local b = ui_element_for_matrix:new()
    b.text = "b"

    local c = ui_element_for_matrix:new()
    c.text = "a"

    return {
        {deepcopy(a)},
        {deepcopy(a)},{deepcopy(b)},
        {deepcopy(a)},{deepcopy(b)},{deepcopy(c)},
    }
end


function create_ui_list(father,pos,layer,space_betwen_elements,ui_element_matrix)
    local ret = {
        core_obj = {},

        position = deepcopy(pos),
        space_betwen_elements = deepcopy(space_betwen_elements),

        ui_element_matrix = deepcopy(ui_element_matrix),
        object_matrix = {},
    }

    function ret:START()
        
    end

    function ret:UPDATE()
        
    end

    function ret:END()
        if self.core_obj.object_ptr ~= nil then
            remove_object(self.core_obj.object_ptr)
        end
    end

    return ret
end