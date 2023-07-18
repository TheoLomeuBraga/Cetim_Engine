require("TMP_libs.definitions")
require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.game_object")
require("TMP_libs.objects.time")

require("TMP_libs.objects.layers_table")
require("TMP_libs.objects.render_layer")




require("math")
json = require("libs.json")






require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.objects.material")
require("TMP_libs.objects.window")














require("TMP_libs.objects.global_data")
require("TMP_libs.short_cuts.create_camera")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.objects.layers_table")
require("TMP_libs.objects.vectors")

cam = {}
layers = layers_table:new_3D()

demos_list = {"text","2D","3D","sound","sprite"}
demo = nil
function load_demo(demo_name)
    if demo ~= nil then
        demo:END()
    end
    demo = nil

    demo = require("tech_demos." .. demo_name)
    demo:START(layers)
end

function START()
    
    print("core iniciando")
    
    window.resolution.x = 720
    window.resolution.y = 720
    window:set()
    
    global_data:set_var("core_object_ptr",this_object_ptr)

    layers:create()

    cam = create_camera_perspective(layers.camera,{x=0,y=0,z=-10},{x=0,y=0,z=0},90,0.1,100)
    cam:add_component(components.lua_scripts)
    cam.components[components.lua_scripts]:add_script("game_scripts/free_camera")

    load_demo(demos_list[1])
    
end

require("TMP_libs.objects.input")
function UPDATE()

    demo:UPDATE()

    keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.q])
    keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.e])

    if keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.delete]) == 1  then
        window:close()
    end
end

function COLLIDE(collision_info)
end

function END()

    window:close()
end
