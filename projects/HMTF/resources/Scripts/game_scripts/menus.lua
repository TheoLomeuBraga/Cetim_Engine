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

menu_type = "pause"

menu_objects = {
    back_ground_image = {},
    title = {},
    start_button = {},
    config_button = {},
    exit_button = {},
}



in_menu = false

function clean_menus()
    for key, value in pairs(menu_objects) do
        if value.END ~= nil then
            value:END()
        end
    end
    menu_objects = nil
    menu_objects = {}
end

function go_to_menu(menu_name)
    menu_type = menu_name
    set_menu()
end

function start()
    go_to_menu("start")
end

function config()
    go_to_menu("config")
end

function new_game()
end

function load_game()
end

function exit_main_menu()
    go_to_menu("main")
end

function exit_pause_menu()
    go_to_menu("páuse")
end

function exit()
    window:close()
end

function set_menu()
    clean_menus()
    
    local style = ui_style:new()

    if menu_type == "pause" then
        style.border_size = 0
        style.color = { r = 0, g = 0.2, b = 0.2, a = 1 }
        style.color_click = { r = 0, g = 0, b = 0, a = 0 }

        if in_main_menu ~= 0 then
            menu_objects.back_ground_image = create_ui(this_object.object_ptr, { x = -1, y = 1, z = 0 }, { x = 2, y = 2, z = 2 }, 4,style, "", 0, "resources/Textures/null.png", nil, ui_category.display)
        end



        style.color = { r = 0, g = 0, b = 0, a = 0 }
        menu_objects.title = create_ui(this_object.object_ptr, { x = -1, y = 1.75, z = 0 }, { x = 2, y = 2, z = 2 },4, style, "HMTF", 0.2, "resources/Textures/null.png", nil, ui_category.display)

        style.color_hover = { r = 0, g = 0, b = 0, a = 0 }
        style.text_color = { r = 0.25, g = 1, b = 1, a = 1 }
        style.border_size = 0.1
        style.border_color = { r = 0, g = 0, b = 0, a = 0 }
        style.border_color_hover = { r = 1, g = 1, b = 1, a = 1 }
        menu_objects.start_button = create_ui(this_object.object_ptr, { x = -0.25, y = 0.5, z = 0 }, { x = 0.5, y = 0.25,z = 2 }, 4, style, "START", 0.075, "resources/Textures/null.png", start, ui_category.button)

        style.text_color = { r = 1, g = 1, b = 0, a = 1 }
        menu_objects.config_button = create_ui(this_object.object_ptr, { x = -0.3, y = 0, z = 0 }, { x = 0.6, y = 0.25, z = 2 },4, style, "CONFIG", 0.075, "resources/Textures/null.png", config, ui_category.button)

        style.text_color = { r = 1, g = 0, b = 0, a = 1 }
        menu_objects.exit_button = create_ui(this_object.object_ptr, { x = -0.2, y = -0.5, z = 0 }, { x = 0.4, y = 0.25, z = 2 }, 4, style, "EXIT", 0.075, "resources/Textures/null.png", exit, ui_category.button)

    elseif menu_type == "start" then
        
    elseif menu_type == "config" then

    end
end

function START()
    local layers = global_data:get_var("layers")

    this_object = game_object:new(this_object_ptr)

    

    set_menu()
end

function UPDATE()
    global_data:set_var("in_menu", true)

    for key, value in pairs(menu_objects) do
        if value.UPDATE ~= nil then
            value:UPDATE()
        end
    end
end

function END()
    global_data:set_var("in_menu", false)
    clean_menus()
end

function COLLIDE(collision_info)
end
