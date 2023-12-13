require("components.extras")
require("objects.game_object")
require("components.transform")
require("components.render_shader")
require("components.audio_source")
require("short_cuts.create_render_shader")


require("objects.input")
require("objects.window")



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

        text_location_x = render_text_location.center,
        text_location_y = render_text_location.center,

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

--deprecated

function create_ui(father, pos, sca, layer, style, text,text_size, image, click_function,category)
    --object
    local main_obj_ptr = create_object(father)
    
    local button_obj_ptr = create_object(main_obj_ptr)
    local text_obj_ptr = create_object(main_obj_ptr)
    

    local ret = {
        main_obj = game_object(main_obj_ptr),
        button_obj = game_object(button_obj_ptr),
        text_obj = game_object(text_obj_ptr),
        text = text,
        image = image,
        click_function = click_function,
        style = style,
        hover = false,
        category = category,

        base_position = deepcopy(pos),
        base_scale = deepcopy(sca),

        enter_last_frame = true,
        click_last_frame = true,
        click = true,
        insertion_mode = false,
        insertion_mode_last_frame = false,
    }

    --transform
    function ret:set_transform( pos, sca)
        self.main_obj.components.transform.is_ui = true
        self.button_obj.components.transform.is_ui = true
        self.text_obj.components.transform.is_ui = true

        self.button_obj.components.transform.position = deepcopy(pos)
        self.button_obj.components.transform.scale = deepcopy(sca)

        if category == 3 then
            self.text_obj.components.transform.position = {x= pos.x+(sca.x * style.border_size / 2),y=pos.y + (sca.y * style.border_size / 2),z=0}
        else
            self.text_obj.components.transform.position = {x= (sca.x / 2) + pos.x,y=-(sca.y / 2) + pos.y - (text_size / 2),z=0}
        end
        
        self.text_obj.components.transform.scale = {x=text_size,y=text_size,z=text_size}

        self.main_obj.components.transform:set()
        self.button_obj.components.transform:set()
        self.text_obj.components.transform:set()
    end

    ret:set_transform( pos, sca)

    --style

    if category == 3 then
        ret.text_obj.components.render_text.text_location_x = render_text_location.right
        ret.text_obj.components.render_text.text_location_y = render_text_location.down
    else
        ret.text_obj.components.render_text.text_location_x = render_text_location.center
        ret.text_obj.components.render_text.text_location_y = render_text_location.center
    end
    
    ret.text_obj.components.render_text.text = text
    ret.text_obj.components.render_text.layer = layer
    ret.text_obj.components.render_text:set()

    ret.button_obj.components.render_shader.layer = layer
    ret.button_obj.components.render_shader:set()

    ret.button_obj.components.render_shader:set()

    function ret:set_style(style)
        self.style = deepcopy(style)

        local render_shader_mat = matreial:new()
        render_shader_mat.shader = "resources/Shaders/button"
        render_shader_mat.color = deepcopy(self.style.color)
        render_shader_mat.inputs["border_color_x"] = self.style.border_color_r
        render_shader_mat.inputs["border_color_y"] = self.style.border_color_g
        render_shader_mat.inputs["border_color_z"] = self.style.border_color_b
        render_shader_mat.inputs["border_color_w"] = self.style.border_color_a

        render_shader_mat.inputs["border_size"] = self.style.border_size

        --colors

        if image == nil or image == "" then
            render_shader_mat.textures[1] = "resources/Textures/white.png"
        else
            render_shader_mat.textures[1] = image
        end

        self.button_obj.components.render_shader.material = deepcopy(render_shader_mat)
        self.button_obj.components.render_shader:set()

        self.text_obj.components.render_text.font = self.style.text_font
        
        if text ~= nil and text ~= "" then
            local render_text_mat = matreial:new()
            render_text_mat.shader = "resources/Shaders/text"
            render_text_mat.color = deepcopy(self.style.text_color)

            
            self.text_obj.components.render_text.material = deepcopy(render_text_mat)
            
            

            self.text_obj.components.render_text:set()
        end
        
    end

    ret:set_style(style)

    function ret:set_text(new_text)
        self.text_obj.components.render_text.text = new_text
        self.text_obj.components.render_text:set()
        self.text = new_text
    end

    function ret:UPDATE()
        window:get()

        if self.category ~= 0 then
            local mouse_pos = {
                x = keys_axis:get_input(input_devices.mouse, "normalized_x"),
                y = keys_axis:get_input(input_devices.mouse, "normalized_y"),
            }
    
            self.button_obj.components.transform:get()
            local pos  = deepcopy(self.button_obj.components.transform.position)
            pos.x      = (pos.x + 1) / 2
            pos.y      = 1 - ((pos.y + 1) / 2) 
            local sca  = deepcopy(self.button_obj.components.transform.scale)
            sca.x      = sca.x / 2
            sca.y      = sca.y / 2
            
            self.hover = (mouse_pos.x > pos.x and mouse_pos.x < (pos.x + sca.x)) and (mouse_pos.y > pos.y and mouse_pos.y < (pos.y + sca.y))
    
            self.click = keys_axis:get_input(input_devices.mouse, "left") == 1
            
    
            if  not self.hover then
                self.button_obj.components.render_shader.material.color = deepcopy(self.style.color)
                self.button_obj.components.render_shader.material.inputs["border_color_x"] = self.style.border_color.r
                self.button_obj.components.render_shader.material.inputs["border_color_y"] = self.style.border_color.g
                self.button_obj.components.render_shader.material.inputs["border_color_z"] = self.style.border_color.b
                self.button_obj.components.render_shader.material.inputs["border_color_w"] = self.style.border_color.a
            elseif  self.hover and not self.click then
                self.button_obj.components.render_shader.material.color = deepcopy(self.style.color_hover)
                self.button_obj.components.render_shader.material.inputs["border_color_x"] = self.style.border_color_hover.r
                self.button_obj.components.render_shader.material.inputs["border_color_y"] = self.style.border_color_hover.g
                self.button_obj.components.render_shader.material.inputs["border_color_z"] = self.style.border_color_hover.b
                self.button_obj.components.render_shader.material.inputs["border_color_w"] = self.style.border_color_hover.a
            elseif self.hover and  self.click then
                self.button_obj.components.render_shader.material.color = deepcopy(self.style.color_click)
                self.button_obj.components.render_shader.material.inputs["border_color_x"] = self.style.border_color_click.r
                self.button_obj.components.render_shader.material.inputs["border_color_y"] = self.style.border_color_click.g
                self.button_obj.components.render_shader.material.inputs["border_color_z"] = self.style.border_color_click.b
                self.button_obj.components.render_shader.material.inputs["border_color_w"] = self.style.border_color_click.a
            end
            self.button_obj.components.render_shader:set()
        else
            self.text_obj.components.render_text.text = self.text
            self.text_obj.components.render_text:set()
        end
        

        if  self.category == 1 or self.category == 3 then
            if self.hover and  self.click and not self.click_last_frame and self.click_function ~= nil then
                self.click_function()
            end
            self.click_last_frame = self.click

            self.text_obj.components.render_text.text = self.text
            self.text_obj.components.render_text:set()
        end

        if self.category == 2 then
            
            if self.hover and self.click and not self.click_last_frame and self.click_function ~= nil then
                
                self.insertion_mode = not self.insertion_mode
                keys_axis:set_text_input_geter(self.insertion_mode)
                
                if keys_axis:get_text_input() ~= "" and keys_axis:get_text_input() ~= "" then
                    self.click_function(keys_axis:get_text_input())
                end
                
            end
            
            if self.click_function ~= nil and ( (self.insertion_mode and not self.hover and self.click) or keys_axis:get_input(input_devices.keyboard, "enter") == 1 and not self.enter_last_frame and self.insertion_mode)  then

                self.insertion_mode = false
                keys_axis:set_text_input_geter(self.insertion_mode)

                if keys_axis:get_text_input() ~= "" and keys_axis:get_text_input() ~= "" then
                    self.click_function(keys_axis:get_text_input())
                end

                
                
            end

            local keyboard_text = ""

            if self.insertion_mode then
                keyboard_text = keys_axis:get_text_input()
            end

            
            
            if keyboard_text ~= "" then
                self.text_obj.components.render_text.text = keyboard_text
            else
                self.text_obj.components.render_text.text = self.text
            end
            
            self.text_obj.components.render_text:set()

            self.insertion_mode_last_frame = self.insertion_mode
            self.click_last_frame = self.click
            self.enter_last_frame = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.enter]) == 1

        end

        
        

        
    end

    function ret:END()
        if self.main_obj.object_ptr ~= nil then
            
            remove_object(self.main_obj.object_ptr)
            self.main_obj.object_ptr = nil
        end
    end

    return ret
end

--new



function create_ui_element(father,ui_type,position,scale,text,interaction_function_name,styles)

    local ret = game_object(create_object((father)))
    local ui_manager = ret.components.ui_component

    ui_manager.layer = 5

    ui_manager.type = ui_type

    ui_manager.id = ret.object_ptr

    ui_manager.position = deepcopy(position)
    ui_manager.scale = deepcopy(scale)
    ui_manager.text = text

    ui_manager.text_location_x = render_text_location.center
    ui_manager.text_location_y = render_text_location.center

    if interaction_function_name ~= "" and interaction_function_name ~= nil then
        ui_manager:set_interaction_function(interaction_function_name)
    end
    
    
    if styles[1] == nil then
        ui_manager.normal_style = deepcopy(styles)
        ui_manager.hover_style = ui_manager.normal_style
        ui_manager.click_style = ui_manager.normal_style
    else
        ui_manager.normal_style = deepcopy(styles[1])
        ui_manager.hover_style = ui_manager.normal_style
        ui_manager.click_style = ui_manager.normal_style
        if styles[2] ~= nil then
            ui_manager.hover_style = deepcopy(styles[2])
        end
        if styles[3] ~= nil then
            ui_manager.click_style = deepcopy(styles[3])
        end
    end

    ui_manager:set()

    return ret
end

function generate_arows(arg,spaces_per_char)
    if spaces_per_char == nil then
        spaces_per_char = 1
    end
    local ret = "> "
    for i = 1, #arg * spaces_per_char, 1 do
        ret = ret .. "  "
    end
    ret = ret .. " <"
    return ret
end

function create_ui_element_with_arows(father,ui_type,position,scale,text,interaction_function_name,style,arow_style,arow_func)
    local ret1 = create_ui_element(father,ui_type,position,scale,text,nil,style)
    if arow_style == nil then
        arow_style = deepcopy(style)
    end
    if arow_func == nil then
        arow_func = generate_arows
    end
    local ret2 = create_ui_element(father,ui_type,position,scale,arow_func(text),interaction_function_name,{deepcopy(empty_style),arow_style})
    return {ret1,ret2}
end