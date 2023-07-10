require("TMP_libs.definitions")
require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.game_object")
require("TMP_libs.objects.time")
require("TMP_libs.layers_table")
require("TMP_libs.objects.render_layer")




require("math")
json = require("libs.json")


require("TMP_libs.short_cuts.load_2D_map")
require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.fps_counter")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.material")
require("TMP_libs.objects.window")
require("TMP_libs.objects.input")
require("TMP_libs.objects.scene_3D")
require("TMP_libs.objects.global_data")



function START()
    
    print("core iniciando")
    
    window.resolution.x = 720
    window.resolution.y = 720
    window:set()
    
    global_data:set_var("core_object_ptr",this_object_ptr)

    
end

function UPDATE()

    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.delete]) == 1  then
        window:close()
    end
end

function COLLIDE(collision_info)
end

function END()
    window:close()
end
