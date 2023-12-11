require("definitions")
require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.game_object")
require("objects.global_data")
require("short_cuts.create_ui")

local serializer = require("libs.serialize")

mensage = nil
mensage_index = nil

local mensage_list_index = 1
local mensage_list = nil

interacting = false
local first_frame = false

dialog_box_father = nil
dialog_box = nil

function interact(args)
    mensage_list_index = 1

    global_data:set("pause", 1)
    global_data:set("interacting", 1)

    if mensage_index ~= nil then
        local mensage_indexs = splitString(mensage_index, ":")
        local localization_data = serializer.load_table_json(global_data:get_var("localization_file"))
        mensage_list = splitString(localization_data[mensage_indexs[1]][mensage_indexs[2]], "\n")
    end

    --[[
    local style = ui_style:new()
    style.color = { r = 0, g = 0, b = 0, a = 1 }
    style.text_color = { r = 1, g = 1, b = 1, a = 1 }
    style.border_size = 0
    style.text_location_x = render_text_location.left
    style.text_location_y = render_text_location.top

    if mensage ~= nil then
        dialog_box_father = create_object(global_data:get("layers").hud)
        dialog_box = create_ui(dialog_box_father, { x = -1, y = -0.5, z = 0 }, { x = 2, y = 0.5, z = 2 }, 5, style,
            mensage, 0.075, "resources/Textures/white.png", nil, ui_category.progrecive_text_fild)
    elseif mensage_list ~= nil then
        dialog_box_father = create_object(global_data:get("layers").hud)
        dialog_box = create_ui(dialog_box_father, { x = -1, y = -0.5, z = 0 }, { x = 2, y = 0.5, z = 2 }, 5, style,
            mensage_list[mensage_list_index], 0.075, "resources/Textures/white.png", nil,
            ui_category.progrecive_text_fild)
    end
    ]]
    local adv_ui = advanced_ui_style()
    adv_ui.text_color = { r = 1, g = 1, b = 1, a = 1 }
    adv_ui.background_color = { r = 0, g = 0, b = 0, a = 1 }
    adv_ui.border_color = { r = 0, g = 0, b = 0, a = 1 }
    adv_ui.border_size = 0.1
    adv_ui.background_image = "resources/Textures/null.png"
    adv_ui.border_image = "resources/Textures/null.png"
    adv_ui.text_size = 0.05

    dialog_box_father = create_object(global_data:get("layers").hud)
    dialog_box = create_advanced_ui_element(dialog_box_father, ui_types.common, { x = 0.5, y = 0.2 }, { x = 0.9, y = 0.5 },
        mensage_list[mensage_list_index], nil, adv_ui)
    dialog_box.components.ui_component.text_location_x = render_text_location.right
    dialog_box.components.ui_component.text_location_y = render_text_location.top
    dialog_box.components.ui_component:set()
end

function stop_interact()
    global_data:set("pause", 0)
    global_data:set("interacting", 0)
    first_frame = false

    remove_object(dialog_box_father)
end

function next_interaction()
    if mensage ~= nil then
        stop_interact()
    elseif mensage_list ~= nil then
        mensage_list_index = mensage_list_index + 1


        if mensage_list_index > tablelength(mensage_list) then
            stop_interact()
        else
            remove_object(dialog_box_father)
            --[[
            local style = ui_style:new()
            style.color = { r = 0, g = 0, b = 0, a = 1 }
            style.text_color = { r = 1, g = 1, b = 1, a = 1 }
            style.border_size = 0
            style.text_location_x = render_text_location.left
            style.text_location_y = render_text_location.top
            dialog_box_father = create_object(global_data:get("layers").hud)
            dialog_box = create_ui(dialog_box_father, { x = -1, y = -0.5, z = 0 }, { x = 2, y = 0.5, z = 2 }, 5, style,
                mensage_list[mensage_list_index], 0.075, "resources/Textures/white.png", nil,
                ui_category.progrecive_text_fild)
            ]]

            local adv_ui = advanced_ui_style()
            adv_ui.text_color = { r = 1, g = 1, b = 1, a = 1 }
            adv_ui.background_color = { r = 0, g = 0, b = 0, a = 1 }
            adv_ui.border_color = { r = 0, g = 0, b = 0, a = 1 }
            adv_ui.border_size = 0.1
            adv_ui.background_image = "resources/Textures/null.png"
            adv_ui.border_image = "resources/Textures/null.png"
            adv_ui.text_size = 0.05

            dialog_box_father = create_object(global_data:get("layers").hud)
            dialog_box = create_advanced_ui_element(dialog_box_father, ui_types.common, { x = 0.5, y = 0.2 },{ x = 0.9, y = 0.5 }, mensage_list[mensage_list_index], nil, adv_ui)
            dialog_box.components.ui_component.text_location_x = render_text_location.right
            dialog_box.components.ui_component.text_location_y = render_text_location.top
            dialog_box.components.ui_component:set()
        end
    end
end

function START()
end

function UPDATE()
    local inputs = global_data:get("inputs")
    local inputs_last_frame = global_data:get("inputs_last_frame")

    if global_data:get("interacting") > 0 and ((inputs.interact > 0 and not (inputs_last_frame.interact > 0)) or (inputs.action_1 > 0 and not (inputs_last_frame.action_1 > 0))) then
        if first_frame then
            next_interaction()
        else
            first_frame = true
        end
    end
end

function COLLIDE(collision_info)
end

function END()
end
