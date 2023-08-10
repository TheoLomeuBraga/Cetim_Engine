

local menu = {
    menu_obj = {},
}

function menu:START()

end

function menu:UPDATE()

end

function menu:END()
    clear_memory()
    remove_object(menu.menu_obj.object_ptr)
end

return menu