require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.global_data")

menu = {
    open = function ()
        local ret = game_object:new(create_object(global_data:get_var("layers").hud))
        ret:add_component(components.transform)
        ret.components[components.transform]:set()
        ret:add_component(components.lua_scripts)
        ret.components[components.lua_scripts]:add_script("game_scripts/menus")
        ret.components[components.lua_scripts]:set_variable("game_scripts/menus", "menu_selectred", "pause")
    end,

    close = function (menu_obj)
        if menu_obj.object_ptr ~= nil then
            remove_object(menu_obj.object_ptr)
        end
        return {}
    end,
}

cenary_bilders = {
    
    player_part = function (father, obj_data)
    end,
    player = function (player_obj, ceane_data)
    end,

    scene_part = function (father, obj_data)
    end,
    scene = function (player_obj, ceane_data)
    end,
}