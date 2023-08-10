require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.short_cuts.create_ui")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_sound")

local menu = {
    menu_obj = {},
}

function menu:START(father,args)

end

function menu:UPDATE()

end

function menu:END()
    --remove_object(config_menu.menu_obj.object_ptr)
end

return menu