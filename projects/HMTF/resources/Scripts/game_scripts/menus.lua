


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


style = {}

menu_obj = {}

this_object = {}

menu_type = "main"

back_ground_image = {}
title = {}

start_button = {}
config_button = {}
exit_button = {}

local core_obj = {}

function start()
    --local core_obj = game_object:new(global_data:get_var("core_object_ptr"))
    --core_obj.components[components.lua_scripts]:call_function("core","load_sceane",{name="text"})
end

function config()
end

function exit_config()
end

function exit()
    window:close()
end

in_menu = false



function START()
    
    

    local layers = global_data:get_var("layers")

    this_object = game_object:new(this_object_ptr)

    style = ui_style:new()

        if menu_type == "main" or menu_type == "pause" then
            style.border_size = 0
            style.color = {r=0,g=0.2,b=0.2,a=1}
            style.color_click = {r=0,g=0,b=0,a=0}

            if menu_type == "main" then
                back_ground_image = create_ui(this_object.object_ptr, {x=-1,y=1,z=0}, {x=2,y=2,z=2}, 4, style, "",0, "resources/Textures/null.png", nil,ui_category.display)
            end
            


            style.color = {r=0,g=0,b=0,a=0}
            title = create_ui(this_object.object_ptr, {x=-1,y=1.75,z=0}, {x=2,y=2,z=2}, 4, style, "HMTF",0.2, "resources/Textures/null.png", nil,ui_category.display)

            style.color_hover = {r=0,g=0,b=0,a=0}
            style.text_color  = {r=0.25,g=1,b=1,a=1}
            style.border_size = 0.1
            style.border_color = {r=0,g=0,b=0,a=0}
            style.border_color_hover = {r=1,g=1,b=1,a=1}
            start_button = create_ui(this_object.object_ptr, {x=-0.25,y=0.5,z=0}, {x=0.5,y=0.25,z=2}, 4, style, "START",0.075, "resources/Textures/null.png", start,ui_category.button)

            style.text_color  = {r=1,g=1,b=0,a=1}
            config_button = create_ui(this_object.object_ptr, {x=-0.3,y=0,z=0}, {x=0.6,y=0.25,z=2}, 4, style, "CONFIG",0.075, "resources/Textures/null.png", config,ui_category.button)

            style.text_color  = {r=1,g=0,b=0,a=1}
            exit_button = create_ui(this_object.object_ptr, {x=-0.2,y=-0.5,z=0}, {x=0.4,y=0.25,z=2}, 4, style, "EXIT",0.075, "resources/Textures/null.png", exit,ui_category.button)
        end
    
    
end

function UPDATE()

    global_data:set_var("in_menu", in_menu)
    if not in_menu then
        start_button:UPDATE()
        config_button:UPDATE()
        exit_button:UPDATE()
    end
    
end

function END()
    remove_object(this_object.object_ptr)
end


function COLLIDE(collision_info)
end



