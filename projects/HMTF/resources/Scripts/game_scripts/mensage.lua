require("engine_libs.definitions")
require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")
require("engine_libs.objects.game_object")
require("engine_libs.objects.global_data")
require("engine_libs.short_cuts.create_ui")


mensage = "hello world"

interacting = false
local first_frame = false

dialog_box_father = nil
dialog_box = nil

function interact(args)
    
    global_data:set("pause",1)
    interacting = true

    local style = ui_style:new()
    style.text_location_x = render_text_location.left
    style.text_location_y = render_text_location.top
    dialog_box_father = create_object(global_data:get("layers").hud)
    dialog_box = create_ui(dialog_box_father, { x = -1, y = -0.5, z = 0 }, { x = 2, y = 0.5,z = 2 }, 4, style, mensage, 0.075, "resources/Textures/white.png", nil, ui_category.progrecive_text_fild)
    
end

function stop_interact()

    global_data:set("pause",0)
    interacting = false
    first_frame = false

    remove_object(dialog_box_father)

end

function next_interaction()
    stop_interact()
end

function START()
end


function UPDATE()
    
    local inputs = global_data:get("inputs")
    local inputs_last_frame = global_data:get("inputs_last_frame")
    
    if interacting and (inputs.interact > 0 and inputs_last_frame.interact < 1) or (inputs.action_1 > 0 and inputs_last_frame.action_1 < 1) then
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
