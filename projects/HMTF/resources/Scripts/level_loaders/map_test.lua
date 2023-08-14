require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_collision")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.objects.time")
require("TMP_libs.objects.scene_3D")
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
