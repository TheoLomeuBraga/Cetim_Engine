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

