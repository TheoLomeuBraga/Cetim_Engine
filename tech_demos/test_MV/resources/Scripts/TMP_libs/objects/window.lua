function get_set_window(get_set,object)
end
function close()
end
window = {resolution = {x = 256,y = 224},full_screen = false}
function window:get()
    --self.resolution.x,self.resolution.y,self.full_screen = get_window()
    new_window = get_set_window(get_lua)
    self.resolution.x = new_window.resolution.x
    self.resolution.y = new_window.resolution.y
    self.full_screen = new_window.full_screen
end
function window:set()
    get_set_window(set_lua,deepcopyjson(window))
end
function window:close()
    close()
end