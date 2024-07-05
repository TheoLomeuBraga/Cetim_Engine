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
    global_data.pause = 1
    global_data.player_object_ptr = nil
    
    local layers = global_data.layers
    menu.camera_obj = create_camera_perspective(layers.camera, { x = 0, y = 0, z = -10 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    set_lisener_object(menu.camera_obj.object_ptr)

    menu.menu_obj = game_object(create_object(layers.hud))
    menu.menu_obj.components.transform:set()
    

    menu.menu_obj.components.lua_scripts:add_script("game_scripts/menus")
    menu.menu_obj.components.lua_scripts:set_variable("game_scripts/menus", "in_main_menu", 1)
    menu.menu_obj.components.lua_scripts:set_variable("game_scripts/menus", "menu_selected", "main_menu")

    game_object(global_data.core_object_ptr).components.lua_scripts.scripts["core"].functions.set_background_image({ path = "Textures/test_sb.svg", color = { r = 1, g = 1, b = 1 } })

end

function menu:UPDATE()

end

function menu:END()
    remove_object(menu.camera_obj.object_ptr)
    remove_object(menu.menu_obj.object_ptr)
    clean_memory()
end

return menu