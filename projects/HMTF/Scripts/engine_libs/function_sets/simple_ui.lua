register_function_set("ui")
require("objects.global_data")

function simple_ui_transform(position, scale, is_ui, rotation)
    if position == nil then
        position = { x = 0, y = 0, z = 0 }
    end
    if scale == nil then
        scale = { x = 1, y = 1, z = 1 }
    end
    if is_ui == nil then
        is_ui = true
    end
    if rotation == nil then
        rotation = { x = 0, y = 0, z = 0 }
    end

    local oposit_bool = function(b)
        if b then
            return false
        end
        return true
    end

    return {
        is_3D = oposit_bool(is_ui),
        position = position,
        scale = scale,
        rotation = rotation
    }
end

function simple_ui_style(color, border_size, border_color, text_color)
    if color == nil then
        color = { r = 0.5, g = 0.5, b = 0.5, a = 1 }
    end
    if border_size == nil then
        border_size = 0
    end
    if border_color == nil then
        border_color = { r = 0, g = 0, b = 0, a = 1 }
    end
    if text_color == nil then
        text_color = { r = 0, g = 0, b = 0, a = 1 }
    end
    return {
        color = color,
        image = "Textures/white.png",
        inputs = {},
        is_mesh = false,
        roundnes = 0,
        skew = 0.0,
        border_size = 0.01,
        border_color = border_color,
        text_color = text_color,
        text_font = "Fonts/OpenSans.ttf",
        text_size = 1.0,
    }
end

local base_styles = {
    simple_ui_display = simple_ui_style(),
}

function set_simple_ui_display(style)
    base_styles["simple_ui_display"] = style
end

function simple_ui_display(ui_transform, text, ui_style)
    local ui_info = {}
    for key, value in pairs(ui_transform) do
        ui_info[key] = ui_transform[key]
    end

    if text == nil then
        ui_info.text = ""
    else
        ui_info.text = text
    end

    if ui_style == nil then
        ui_style = base_styles["simple_ui_display"]
    end

    for key, value in pairs(ui_style) do
        ui_info[key] = ui_style[key]
    end

    c_simple_ui_display(ui_info)
end

function simple_ui_lable(ui_transform, ui_style)
    simple_ui_display(ui_transform, "", ui_style)
end

function simple_ui_text(ui_transform, text, ui_style)
    if ui_style == nil then
        ui_style = simple_ui_style()
    end
    ui_style.color.a = 0
    ui_style.border_color.a = 0
    simple_ui_display(ui_transform, text, ui_style)
end

function update_simple_ui()
    if global_data.inputs ~= nil then
        local inputs = global_data.inputs
        local inputs_last_frame = global_data.inputs_last_frame

        local cursor_movement = {x=inputs.mouse_pos_x-cursor_pos_last_frame.x,y=cursor_pos_last_frame.y-inputs.mouse_pos_y}
        set_selection_state({x=inputs.mouse_pos_x,y=inputs.mouse_pos_y},cursor_movement,-1,inputs.action_1)
        cursor_pos_last_frame = {x=inputs.mouse_pos_x,y=inputs.mouse_pos_y}
    end 
    
end

local simple_ui_cursor_position = { x = 0, y = 0 }
local simple_ui_cursor_movement = { x = 0, y = 0 }
local simple_ui_joystick_selection = -1
local simple_ui_confirm_last_frame = 0
local simple_ui_confirm = 0
function set_selection_state(cursor_position, ursor_movement, ui_joystick_id, confirm)
    simple_ui_cursor_position = cursor_position
    simple_ui_cursor_movement = ursor_movement
    simple_ui_joystick_selection = ui_joystick_id
    simple_ui_confirm_last_frame = simple_ui_confirm
    simple_ui_confirm = confirm
end

function is_cliked(ui_transform, ui_joystick_id)
    if simple_ui_confirm_last_frame == 0 then
        if ui_joystick_id == simple_ui_joystick_selection then
            if simple_ui_confirm == 1 then
                return 1
            else
                return 0.5
            end
        end

        local cpt = { x = (simple_ui_cursor_position.x * 2) - 1, y = (((1 - simple_ui_cursor_position.y) * 2) - 1) }
        if ui_transform.position.x - ui_transform.scale.x < cpt.x and ui_transform.position.x + ui_transform.scale.x > cpt.x and ui_transform.position.y - ui_transform.scale.y < cpt.y and ui_transform.position.y + ui_transform.scale.y > cpt.y then
            if simple_ui_confirm == 1 then
                return 1
            else
                return 0.5
            end
        end
    end
    return 0
end

function is_holded(ui_transform, ui_joystick_id)
    if ui_joystick_id == simple_ui_joystick_selection then
        if simple_ui_confirm == 1 then
            return 1
        else
            return 0.5
        end
    end

    local cpt = { x = (simple_ui_cursor_position.x * 2) - 1, y = (((1 - simple_ui_cursor_position.y) * 2) - 1) }
    if ui_transform.position.x - ui_transform.scale.x < cpt.x and ui_transform.position.x + ui_transform.scale.x > cpt.x and ui_transform.position.y - ui_transform.scale.y < cpt.y and ui_transform.position.y + ui_transform.scale.y > cpt.y then
        if simple_ui_confirm == 1 then
            return 1
        else
            return 0.5
        end
    end

    return 0
end

function simple_ui_button(ui_transform, text, ui_style, ui_joystick_id)
    simple_ui_display(ui_transform, text, ui_style)
    return is_cliked(ui_transform, ui_joystick_id)
end

function simple_ui_slider(ui_transform, text, ui_style, ui_joystick_id)
    simple_ui_display(ui_transform, text, ui_style)
    if is_holded(ui_transform, ui_joystick_id) == 1 then
        return { x = simple_ui_cursor_movement.x, y = simple_ui_cursor_movement.y, z = 0 }
    end
    return { x = 0, y = 0, z = 0 }
end





local ui_next_pos = {x=0,y=0}
cursor_pos_last_frame = {x=0,y=0,z=0}
local reload_last_frame = false
function simple_ui_example()
    
    local ui_transform = simple_ui_transform()
    ui_transform.position = {x=ui_next_pos.x,y=0.30 + ui_next_pos.y,z=0}
    ui_transform.scale = {x=0.25,y=0.05}
    local slider_result = simple_ui_slider(ui_transform,"window",nil,1)
    ui_next_pos.x = ui_next_pos.x + (slider_result.x * 2)
    ui_next_pos.y = ui_next_pos.y + (slider_result.y * 2)
    
    local style = simple_ui_style({r=0.5,g=0.5,b=0.5,a=1}, 0.05, {r=1,g=0,b=0,a=1}, {r=1,g=0,b=0,a=1})
    ui_transform = simple_ui_transform()
    ui_transform.position = {x=ui_next_pos.x,y=ui_next_pos.y,z=0}
    ui_transform.scale = {x=0.25,y=0.25}
   
    if simple_ui_button(ui_transform,"push me",style,1) == 1 then
        window:close()
    end
end