require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")
require("engine_libs.objects.global_data")

menu = {
    obj = nil,
    open = function ()
        if menu.obj == nil then
            menu.obj = game_object:new(create_object(global_data:get_var("layers").hud))
            menu.obj:add_component(components.transform)
            menu.obj.components[components.transform]:set()
            menu.obj:add_component(components.lua_scripts)
            menu.obj.components[components.lua_scripts]:add_script("game_scripts/menus")
            menu.obj.components[components.lua_scripts]:set_variable("game_scripts/menus", "menu_selectred", "pause")
        end
    end,

    close = function (menu_obj)
        if menu.obj.object_ptr ~= nil then
            remove_object(menu.obj.object_ptr)
        end
        menu.obj = nil
    end,
}

cenary_bilders = {

    entity_part = function (father, obj_data)
    end,
    entity = function (entity_obj, ceane_data)
    end,

    scene_part = function (father, obj_data)
    end,
    scene = function (player_obj, ceane_data)
    end,
}