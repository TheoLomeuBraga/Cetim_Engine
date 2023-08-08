require("TMP_libs.components.extras")
require("TMP_libs.objects.game_object")
require("TMP_libs.components.transform")
require("TMP_libs.components.render_shader")
require("TMP_libs.short_cuts.create_render_shader")

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

function create_ui(father,is_ui,pos,sca,layer,style,text,image,click_function)

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
    }

    --transform
    ret.core_obj:add_component(components.transform)
    ret.button_obj:add_component(components.transform)
    ret.text_obj:add_component(components.transform)
    function ret:set_transform(is_ui,pos,sca)

        self.core_obj.components[components.transform].is_ui = is_ui
        self.button_obj.components[components.transform].is_ui = is_ui
        self.text_obj.components[components.transform].is_ui = is_ui

        self.core_obj.components[components.transform].position = deepcopy(pos)
        self.core_obj.components[components.transform].scale = deepcopy(sca)

        self.core_obj.components[components.transform]:set()
        self.button_obj.components[components.transform]:set()
        self.text_obj.components[components.transform]:set()
        
    end
    ret:set_transform(is_ui,pos,sca)

    --style
    ret.button_obj:add_component(components.render_shader)
    --ret.text_obj:add_component(components.render_text)
    function ret:set_style(style)

        local render_shader_mat = matreial:new()
        render_shader_mat.shader = "resources/Shaders/button"
        render_shader_mat.color = deepcopy(style.color)

        --render_shader_mat.inputs[1] = style.border_size
        --render_shader_mat.inputs[2] = style.border_roundnes

        if image == nil or image == "" then
            render_shader_mat.textures[1] = "resources/Textures/white.png"
        else
            render_shader_mat.textures[1] = image
        end

        ret.button_obj.components[components.render_shader].material = deepcopy(render_shader_mat)
        ret.button_obj.components[components.render_shader]:set()

        --[[
        if text ~= nil and text ~= "" then
            local render_text_mat = matreial:new()
            render_text_mat.shader = "resources/Shaders/text"
        end
        ]]

    end
    ret:set_style(style)

    

    return ret
end

function create_button(father, is_ui, pos, rot, sca, layer, mat)
    ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].is_ui = is_ui
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    ret:add_component(components.render_shader)
    ret.components[components.render_shader].layer = layer
    ret.components[components.render_shader].material = deepcopy(mat)
    ret.components[components.render_shader]:set()

    return ret
end

function create_text_ui(father, is_ui, pos, rot, sca, layer, mat)
    ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].is_ui = is_ui
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    ret:add_component(components.render_shader)
    ret.components[components.render_shader].layer = layer
    ret.components[components.render_shader].material = deepcopy(mat)
    ret.components[components.render_shader]:set()

    return ret
end

function create_input_fild(father, is_ui, pos, rot, sca, layer, mat)
    ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].is_ui = is_ui
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    ret:add_component(components.render_shader)
    ret.components[components.render_shader].layer = layer
    ret.components[components.render_shader].material = deepcopy(mat)
    ret.components[components.render_shader]:set()

    return ret
end
