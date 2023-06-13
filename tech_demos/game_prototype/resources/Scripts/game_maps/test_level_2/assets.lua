require("TMP_libs.short_cuts.create_camera")

return {
    free_camera = function(father, position, rotation)
        local ret = create_camera_perspective(father, position, rotation, 90, 0.1, 500)
        ret:add_component(components.lua_scripts)
        ret.components[components.lua_scripts]:add_script("game_scripts/free_camera")
        return ret
    end,

    player = function(father, position, rotation)
        local ret = game_object:new(create_object(father))
        ret:add_component(components.transform)
        ret:add_component(components.camera)
        ret.components[components.transform].position = position
        ret.components[components.transform].rotation = rotation
        ret.components[components.camera].type = camera_view_types.perspective
        ret.components[components.camera].orthographc = false
        ret.components[components.camera].zoom = 90
        ret.components[components.camera].ncp = 0.01
        ret.components[components.camera].fcp = 500
        ret.components[components.transform]:set()
        ret.components[components.camera]:set()

        ret:add_component(components.lua_scripts)
        ret.components[components.lua_scripts]:add_script("game_scripts/player")
        ret.components[components.lua_scripts]:set_variable("game_scripts/player", "current_pos", position)
        ret.components[components.lua_scripts]:set_variable("game_scripts/player", "current_rot", rotation)
        return ret
    end
}
