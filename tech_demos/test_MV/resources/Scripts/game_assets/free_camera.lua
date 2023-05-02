require("TMP_libs.short_cuts.create_camera")

return {
    create = function (father,position,rotation)
        local ret = create_camera_perspective(father, position, rotation, 90, 0.1,100)
        ret:add_component(components.lua_scripts)
        ret.components[components.lua_scripts]:add_script("game_scripts/free_camera")
        return ret
    end,
    create_extra = function ()
        
    end
}