require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.input")
require("TMP_libs.objects.time")
require("TMP_libs.objects.global_data")
require("TMP_libs.objects.window")
require("TMP_libs.short_cuts.create_ui")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.objects.global_data")
require("TMP_libs.objects.window")
require("math")



menu_obj = {}

this_object = {}

menu_types = {
    pause = "pause",
    start = "start",
    config = "config",
}

in_main_menu = 0

menu_selectred = "pause"

pause_menu_objects = {}

start_menu_objects = {}

config_menu_objects = {}

in_menu = false

function exit_to_pause_menu()

    menu_selectred = "pause"

    for index, value in pairs(start_menu_objects) do
        value:END()
    end
    start_menu_objects = {}

    for index, value in pairs(config_menu_objects) do
        value:END()
    end
    config_menu_objects = {}

    

end

function new_game()
    print("new_game")
end

function load_game()
    print("load_game")
end

function call_start_menu()
    menu_selectred = "start"

    local style = ui_style:new()
    style.border_size = 0
    style.color = { r = 0.2, g = 0.2, b = 0.2, a = 1 }
    style.color_click = { r = 0, g = 0, b = 0, a = 0 }
    start_menu_objects.back_ground_image = create_ui(this_object.object_ptr, { x = -1, y = 0.6, z = 0 }, { x = 2, y = 1.6, z = 2 }, 4,style, "", 0, "resources/Textures/white.png", nil, ui_category.display)
    
    

    style.text_color = { r = 0, g = 1, b = 1, a = 1 }
    style.color_hover = { r = 0, g = 0, b = 0, a = 0 }
    style.border_size = 0.1
    style.border_color = { r = 0, g = 0, b = 0, a = 0 }
    style.border_color_hover = { r = 1, g = 1, b = 1, a = 1 }
    start_menu_objects.new_game_button = create_ui(this_object.object_ptr, { x = -0.5, y = 0.5, z = 0 }, { x = 1, y = 0.25,z = 2 }, 4, style, "NEW GAME", 0.075, "resources/Textures/white.png", new_game, ui_category.button)

    style.text_color = { r = 1, g = 1, b = 0, a = 1 }
    start_menu_objects.load_game_button = create_ui(this_object.object_ptr, { x = -0.6, y = 0, z = 0 }, { x = 1.2, y = 0.25,z = 2 }, 4, style, "LOAD GAME", 0.075, "resources/Textures/white.png", load_game, ui_category.button)

    style.text_color = { r = 1, g = 0, b = 0, a = 1 }
    start_menu_objects.exit_button = create_ui(this_object.object_ptr, { x = -0.2, y = -0.5, z = 0 }, { x = 0.4, y = 0.25, z = 2 }, 4, style, "BACK", 0.075, "resources/Textures/white.png", exit_to_pause_menu, ui_category.button)
end





function call_config_menu()
    menu_selectred = "config"

    local style = ui_style:new()
    style.border_size = 0
    style.color = { r = 0.25, g = 0.25, b = 0.25, a = 1 }
    style.color_click = { r = 0, g = 0, b = 0, a = 0 }
    start_menu_objects.back_ground_image = create_ui(this_object.object_ptr, { x = -1, y = 0.6, z = 0 }, { x = 2, y = 1.6, z = 2 }, 4,style, "", 0, "resources/Textures/white.png", nil, ui_category.display)

    
    style.text_color = { r = 1, g = 0, b = 0, a = 1 }
    style.color_hover = { r = 0, g = 0, b = 0, a = 0 }
    style.border_size = 0.1
    style.border_color = { r = 0, g = 0, b = 0, a = 0 }
    style.border_color_hover = { r = 1, g = 1, b = 1, a = 1 }
    config_menu_objects.exit_button = create_ui(this_object.object_ptr, { x = -0.2, y = -0.5, z = 0 }, { x = 0.4, y = 0.25, z = 2 }, 4, style, "BACK", 0.075, "resources/Textures/white.png", exit_to_pause_menu, ui_category.button)

end






function exit()
    window:close()
end


function START()

    local layers = global_data:get_var("layers")

    this_object = game_object:new(this_object_ptr)
    
    local style = ui_style:new()

    style.border_size = 0
    style.color = { r = 0, g = 0.2, b = 0.2, a = 1 }
    style.color_click = { r = 0, g = 0, b = 0, a = 0 }

    if in_main_menu ~= 0 then
        pause_menu_objects.back_ground_image = create_ui(this_object.object_ptr, { x = -1, y = 1, z = 0 }, { x = 2, y = 2, z = 2 }, 4,style, "", 0, "resources/Textures/null.png", nil, ui_category.display)
    end

    style.color = { r = 0, g = 0, b = 0, a = 0 }
    pause_menu_objects.title = create_ui(this_object.object_ptr, { x = -1, y = 1.75, z = 0 }, { x = 2, y = 2, z = 2 },4, style, "HMTF", 0.2, "resources/Textures/null.png", nil, ui_category.display)

    style.color_hover = { r = 0, g = 0, b = 0, a = 0 }
    style.text_color = { r = 0.25, g = 1, b = 1, a = 1 }
    style.border_size = 0.1
    style.border_color = { r = 0, g = 0, b = 0, a = 0 }
    style.border_color_hover = { r = 1, g = 1, b = 1, a = 1 }
    pause_menu_objects.start_button = create_ui(this_object.object_ptr, { x = -0.25, y = 0.5, z = 0 }, { x = 0.5, y = 0.25,z = 2 }, 4, style, "START", 0.075, "resources/Textures/null.png", call_start_menu, ui_category.button)

    style.text_color = { r = 1, g = 1, b = 0, a = 1 }
    pause_menu_objects.config_button = create_ui(this_object.object_ptr, { x = -0.3, y = 0, z = 0 }, { x = 0.6, y = 0.25, z = 2 },4, style, "CONFIG", 0.075, "resources/Textures/null.png", call_config_menu, ui_category.button)

    style.text_color = { r = 1, g = 0, b = 0, a = 1 }
    pause_menu_objects.exit_button = create_ui(this_object.object_ptr, { x = -0.2, y = -0.5, z = 0 }, { x = 0.4, y = 0.25, z = 2 }, 4, style, "EXIT", 0.075, "resources/Textures/null.png", exit, ui_category.button)


end

function UPDATE()

    global_data:set_var("in_menu", true)

    if menu_selectred == "pause" then
        for key, value in pairs(pause_menu_objects) do
            if value.UPDATE ~= nil then
                value:UPDATE()
            end
        end
    end
    
    if menu_selectred == "start" then

        if tablelength(start_menu_objects) == 0 then
            call_start_menu()
        end

        for key, value in pairs(start_menu_objects) do
            if value.UPDATE ~= nil then
                value:UPDATE()
            end
        end
    end

    if menu_selectred == "config" then
        if tablelength(config_menu_objects) == 0 then
            call_config_menu()
        end

        for key, value in pairs(config_menu_objects) do
            if value.UPDATE ~= nil then
                value:UPDATE()
            end
        end
    end
    
end

function END()
    global_data:set_var("in_menu", false)
end

function COLLIDE(collision_info)
end
