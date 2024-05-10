register_function_set("ui")

function simple_ui_transform(position,scale,is_ui,rotation)
    if position == nil then
        position = {x=0,y=0,z=0}
    end
    if scale == nil then
        scale = {x=1,y=1,z=1}
    end
    if is_ui == nil then
        is_ui = true
    end
    if rotation == nil then
        rotation = {x=0,y=0,z=0}
    end

    local oposit_bool = function (b)
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

function simple_ui_style(color,border_size,border_color,text_color)
    if color == nil then
        color = {r=0.5,g=0.5,b=0.5,a=1}
    end
    if border_size == nil then
        border_size = 0
    end
    if border_color == nil then
        border_color = {r=0,g=0,b=0,a=1}
    end
    if text_color == nil then
        text_color = {r=0,g=0,b=0,a=1}
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
function simple_ui_display(ui_transform,text,ui_style)

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

function simple_ui_lable(ui_transform,ui_style)
    simple_ui_display(ui_transform,"",ui_style)
end

function simple_ui_text(ui_transform,text,ui_style)
    if ui_style == nil then
        ui_style = simple_ui_style()
    end
    ui_style.color.a = 0
    ui_style.border_color.a = 0
    simple_ui_display(ui_transform,text,ui_style)
end

local simple_ui_cursor_position = {x=0,y=0}
local simple_ui_cursor_movement = {x=0,y=0}
local simple_ui_joystick_selection = -1
local simple_ui_confirm = 0
function set_selection_state(cursor_position,ursor_movement,ui_joystick_id,confirm)
    simple_ui_cursor_position = cursor_position
    simple_ui_cursor_movement = ursor_movement
    simple_ui_joystick_selection = ui_joystick_id
    simple_ui_confirm = confirm
end
function is_selected(ui_transform,ui_joystick_id)

    if ui_joystick_id == simple_ui_joystick_selection then
        if simple_ui_confirm == 1 then
            return 1
        else 
            return 0.5
        end
    end

    local cpt = {x=(simple_ui_cursor_position.x * 2) - 1,y=(((1 - simple_ui_cursor_position.y) * 2) - 1)}
    if ui_transform.position.x - ui_transform.scale.x < cpt.x and ui_transform.position.x + ui_transform.scale.x > cpt.x and ui_transform.position.y - ui_transform.scale.y < cpt.y and ui_transform.position.y + ui_transform.scale.y > cpt.y then
        if simple_ui_confirm == 1 then
            return 1
        else 
            return 0.5
        end
    end

    return 0
end

function simple_ui_button(ui_transform,text,ui_style,ui_joystick_id)
    simple_ui_display(ui_transform,text,ui_style)
    return is_selected(ui_transform,ui_joystick_id)
end

function simple_ui_slider(ui_transform,text,ui_style,ui_joystick_id)
    simple_ui_display(ui_transform,text,ui_style)
    if is_selected(ui_transform,ui_joystick_id) == 1 then
        return {x=simple_ui_cursor_movement.x,y=simple_ui_cursor_movement.y,z=0}
    end
    return {x=0,y=0,z=0}
end

