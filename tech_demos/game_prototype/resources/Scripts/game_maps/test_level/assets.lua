require("TMP_libs.short_cuts.create_camera")

return {
    free_camera = function (father,position,rotation)
        local ret = create_camera_perspective(father, position, rotation, 90, 0.1,500)
        --local ret = create_camera_ortho(father,pos,rot,150,150,1,1,0.1,100)
        ret:add_component(components.lua_scripts)
        ret.components[components.lua_scripts]:add_script("game_scripts/free_camera")
        return ret
    end,

    player = function (father,position,rotation)
        local ret = create_camera_perspective(father, position, rotation, 90, 0.1,500)
        --local ret = create_camera_ortho(father,pos,rot,150,150,1,1,0.1,100)
        ret.components[components.transform]:set()

        ret:add_component(components.lua_scripts)
        ret.components[components.lua_scripts]:add_script("game_scripts/player")
        ret.components[components.lua_scripts]:set_variable("game_scripts/player","current_pos",deepcopy(position))
        ret.components[components.lua_scripts]:set_variable("game_scripts/player","current_rot",deepcopy(rotation))
        return ret
    end,
}