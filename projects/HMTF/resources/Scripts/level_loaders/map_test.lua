require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_collision")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.objects.time")
require("TMP_libs.objects.scene_3D")

local level = {
    camera_obj = {},
    menu = {},
}

function open_menu()
    level.menu = game_object:new(create_object(layers.hud))
    level.menu:add_component(components.transform)
    level.menu.components[components.transform]:set()
    level.menu:add_component(components.lua_scripts)
    level.menu.components[components.lua_scripts]:add_script("game_scripts/menus")
    level.menu.components[components.lua_scripts]:set_variable("game_scripts/menus", "in_main_menu", 0)
    level.menu.components[components.lua_scripts]:set_variable("game_scripts/menus", "menu_selectred", "pause")
end

function close_menu()
    if level.menu.object_ptr ~= nil then
        remove_object(level.menu.object_ptr)
    end
    level.menu = {}
end

function level:START(layers)

    level.camera_obj = create_camera_perspective(layers.camera, { x = 0, y = 0, z = -10 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    set_lisener_object(level.camera_obj.object_ptr)

    
    
end

function level:UPDATE()
end

function level:END()
    close_menu()
    clear_memory()
end

return level
