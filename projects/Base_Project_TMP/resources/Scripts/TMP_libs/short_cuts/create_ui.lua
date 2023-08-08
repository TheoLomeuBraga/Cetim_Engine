require("TMP_libs.components.extras")
require("TMP_libs.objects.game_object")
require("TMP_libs.components.transform")
require("TMP_libs.components.render_shader")
require("TMP_libs.short_cuts.create_render_shader")

style = {}
function style:new()
    return {

        color = { r = 1, g = 0, b = 0, a = 1 },
        color_hover = { r = 0, g = 0, b = 1, a = 1 },
        color_click = { r = 0, g = 0, b = 1, a = 1 },
        background_image = "resources/Textures/white.png",

        text_font = "resources/Fonts/Glowworm Regular.json",
        text_color = { r = 0, g = 1, b = 0, a = 1 },
        text_color_hover = { r = 0, g = 0, b = 1, a = 1 },
        text_color_click = { r = 0, g = 0, b = 1, a = 1 },

        border_size = 0.5,
        border_roundnes = 0.5,
        border_color = { r = 0, g = 0, b = 1, a = 1 },
        border_color_hover = { r = 0, g = 0, b = 1, a = 1 },
        border_color_click = { r = 0, g = 0, b = 1, a = 1 },

    }
end

local ui_object_example = {

    obj = {},

    text = "hello world",

    click_function = nil,

    style = style:new()

}

function create_ui(father,is_ui,pos,sca,text,click_function,style)

    --object
    ret = {
        obj = game_object:new(create_object(father)),
        text = text,
        click_function = click_function,
        style = style
    }

    --transform
    ret.obj:add_component(components.transform)
    function ret:set_transform(is_ui,pos,sca)
        ret.obj.components[components.transform].is_ui = is_ui
        ret.obj.components[components.transform].position = deepcopy(pos)
        ret.obj.components[components.transform].scale = deepcopy(sca)
        ret.obj.components[components.transform]:set()
    end
    ret:set_transform(is_ui,pos,sca)
    
    --button

    

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
