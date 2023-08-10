require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.short_cuts.create_ui")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.objects.scene_3D")
require("TMP_libs.objects.global_data")
require("TMP_libs.objects.window")

local menu = {
    style = {},

    menu_obj = {},

    back_ground_image = {},

    title = {},
    

    start = {},
    config = {},
    exit = {},
}

local core_obj = {}

function start()
    local core_obj = game_object:new(global_data:get_var("core_object_ptr"))
    core_obj.components[components.lua_scripts]:call_function("load_sceane",{name="3D"})
end

function config()
end

function exit_config()
end

function exit()
    window:close()
end

function menu:START(layers)

    menu.menu_obj = game_object:new(create_object(layers.cenary))

    menu.style = ui_style:new()

    menu.style.border_size = 0
    menu.style.color = {r=0,g=0.2,b=0.2,a=1}
    menu.back_ground_image = create_ui(menu.menu_obj.object_ptr, {x=-1,y=1,z=0}, {x=2,y=2,z=2}, 4, menu.style, "",0, "resources/Textures/null.png", nil,ui_category.display)

    menu.style.color = {r=0,g=0,b=0,a=0}
    menu.title = create_ui(menu.menu_obj.object_ptr, {x=-1,y=1.75,z=0}, {x=2,y=2,z=2}, 4, menu.style, "HMTF",0.2, "resources/Textures/null.png", nil,ui_category.display)

    menu.style.text_color  = {r=0.25,g=1,b=1,a=1}
    menu.start = create_ui(menu.menu_obj.object_ptr, {x=-0.25,y=0.5,z=0}, {x=0.5,y=0.25,z=2}, 4, menu.style, "START",0.075, "resources/Textures/null.png", start,ui_category.button)

    menu.style.text_color  = {r=1,g=1,b=0,a=1}
    menu.config = create_ui(menu.menu_obj.object_ptr, {x=-0.3,y=0,z=0}, {x=0.6,y=0.25,z=2}, 4, menu.style, "CONFIG",0.075, "resources/Textures/null.png", config,ui_category.button)

    menu.style.text_color  = {r=1,g=0,b=0,a=1}
    menu.exit = create_ui(menu.menu_obj.object_ptr, {x=-0.2,y=-0.5,z=0}, {x=0.4,y=0.25,z=2}, 4, menu.style, "EXIT",0.075, "resources/Textures/null.png", exit,ui_category.button)

end

function menu:UPDATE()
    menu.start:UPDATE()
    menu.config:UPDATE()
    menu.exit:UPDATE()
end

function menu:END()
    clear_memory()
    remove_object(menu.menu_obj.object_ptr)
end

return menu