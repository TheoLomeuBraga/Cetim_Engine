require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.short_cuts.create_ui")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.objects.scene_3D")
require("TMP_libs.objects.global_data")

local menu = {
    menu_obj = {},

    title = {},
    back_ground_image = {},

    start = {},
    config = {},
    exit = {},
}

local core_obj = {}

function start()
    core_obj = game_object:new(global_data:get_var("core_object_ptr"))
    core_obj.components[components.lua_scripts]:call_function("load_sceane")
end

function config()
end

function exit_config()
end

function exit()
end

function menu:START(layers)

    menu.menu_obj = game_object:new(create_object(layers.cenary))

    

end

function menu:UPDATE()

end

function menu:END()
    clear_memory()
    remove_object(menu.menu_obj.object_ptr)
end

return menu