require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.short_cuts.create_ui")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.objects.scene_3D")

local config_menu = {
    menu_obj = {},
}

function config_menu:START()

end

function config_menu:UPDATE()

end

function config_menu:END()
    --remove_object(config_menu.menu_obj.object_ptr)
end

return config_menu