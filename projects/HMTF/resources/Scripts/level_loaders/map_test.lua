require("engine_libs.short_cuts.create_mesh")
require("engine_libs.short_cuts.create_collision")
require("engine_libs.short_cuts.create_sound")
require("engine_libs.objects.time")
require("engine_libs.objects.scene_3D")
require("resources.playable_scene")

local level = {
    camera_obj = {},
}



function level:START(layers)
    
    level.camera_obj = create_camera_perspective(layers.camera, { x = 0, y = 0, z = 0 }, { x = 0, y = 0, z = 0 }, 90, 0.1, 1000)
    set_lisener_object(level.camera_obj.object_ptr)
    
    
end

function level:UPDATE()
end

function level:END()
    clear_memory()
end

return level
