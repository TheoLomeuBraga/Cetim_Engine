require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.short_cuts.create_ui")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.objects.scene_3D")
require("TMP_libs.objects.global_data")
require("TMP_libs.objects.window")
require("TMP_libs.short_cuts.create_camera")
require("TMP_libs.objects.global_data")

local menu = {
    camera_obj = {},
    menu_obj = {},
    
}

function load_configs()
    
end

function menu:START(layers)
    menu.camera_obj = create_camera_perspective(layers.camera, { x = 0, y = 0, z = -10 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    set_lisener_object(menu.camera_obj.object_ptr)
    set_global_volume(100)
    global_data:set_var("global_volume", 100)

    menu.menu_obj = game_object:new(create_object(layers.hud)) 
    menu.menu_obj:add_component(components.transform)
    menu.menu_obj.components[components.transform]:set()

    menu.menu_obj:add_component(components.lua_scripts)
    menu.menu_obj.components[components.lua_scripts]:add_script("game_scripts/menus")
    
    menu.menu_obj.components[components.lua_scripts]:set_variable("game_scripts/menus", "in_main_menu", 1)
    menu.menu_obj.components[components.lua_scripts]:set_variable("game_scripts/menus", "menu_selectred", "pause")

end

function menu:UPDATE()

end

function menu:END()
    clear_memory()
    remove_object(menu.camera_obj.object_ptr)
    remove_object(menu.menu_obj.object_ptr)
end

return menu