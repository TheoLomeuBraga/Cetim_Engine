register_function_set("window")

window = {resolution = {x = 256,y = 256},full_screen = false}
function window:get()
    --self.resolution.x,self.resolution.y,self.full_screen = get_window()
    new_window = get_set_window(get_lua)
    self.resolution.x = new_window.resolution.x
    self.resolution.y = new_window.resolution.y
    self.full_screen = new_window.full_screen > 0
end
function window:set()
    get_set_window(set_lua,deepcopyjson(self))
end
function window:set_cursor_image(path)
    set_cursor_image(path)
end
function window:enable_cursor(enable)
    enable_cursor(enable)
end

function window:close()
    close()
end