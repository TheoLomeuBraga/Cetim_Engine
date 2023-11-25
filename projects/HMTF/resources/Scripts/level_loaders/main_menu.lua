require("short_cuts.create_text")
require("short_cuts.create_render_shader")
require("short_cuts.create_ui")
require("short_cuts.create_mesh")
require("short_cuts.create_sound")
require("objects.scene_3D")
require("objects.global_data")
require("objects.window")
require("short_cuts.create_camera")
require("objects.global_data")
require("components.component_all")


local menu = {
    camera_obj = {},
    menu_obj = {},
}



function menu:START()
    local layers = global_data:get_var("layers")
    menu.camera_obj = create_camera_perspective(layers.camera, { x = 0, y = 0, z = -10 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    set_lisener_object(menu.camera_obj.object_ptr)

    menu.menu_obj = game_object(create_object(layers.hud))
    menu.menu_obj.components[components.transform]:set()

    menu.menu_obj.components[components.lua_scripts]:add_script("game_scripts/menus")
    menu.menu_obj.components[components.lua_scripts]:set_variable("game_scripts/menus", "in_main_menu", 1)
    menu.menu_obj.components[components.lua_scripts]:set_variable("game_scripts/menus", "menu_selectred", "pause")

end

function menu:UPDATE()

end

function menu:END()
    remove_object(menu.camera_obj.object_ptr)
    remove_object(menu.menu_obj.object_ptr)
    clear_memory()
end

return menu