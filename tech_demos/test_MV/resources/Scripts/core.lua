require("TMP_libs.definitions")
require("TMP_libs.components.component_table")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.game_object")
require("TMP_libs.objects.time")
require("TMP_libs.stystems")
require("TMP_libs.input")
require("TMP_libs.layers_table")
require("TMP_libs.objects.render_layer")


require("math")
json = require("libs.json")


require("TMP_libs.short_cuts.load_2D_map")
require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_camera")
require("TMP_libs.short_cuts.fps_counter")
require("TMP_libs.short_cuts.create_render_shader")

require("TMP_libs.material")
require("all_sceanes")


camera = nil
map_info = nil
background = nil
objects_layesrs = layers_table:new_2D()


fps_count = nil


function clean_layer(ptr)
    obj = game_object:new(ptr)
    obj:get_family()
    --obj:get()
    i = 0
    print("childrens_size",obj.childrens_size)
    while i < obj.childrens_size do
        remove_object(obj.childrens[i])
        i = i + 1
    end
    
end






function set_res()
    window.res.x = 720
    window.res.y = 720
    window:set()
end

function initialize_render_layers()
    --[[
    window.res.x = 720
    window.res.y = 720
    window:set()
    --]]

    renders_layers.layers_size = 4

    renders_layers.layers[1] = render_layer:new()
    renders_layers.layers[1].end_render = false
    renders_layers.layers[1].use_deeph = true
    renders_layers.layers[1].clean_color = false
    renders_layers.layers[1].start_render = true

    renders_layers.layers[2] = render_layer:new()
    renders_layers.layers[2].end_render = false
    renders_layers.layers[2].use_deeph = true
    renders_layers.layers[2].clean_color = false
    renders_layers.layers[2].start_render = false

    renders_layers.layers[3] = render_layer:new()
    renders_layers.layers[3].end_render = true
    renders_layers.layers[3].use_deeph = true
    renders_layers.layers[3].clean_color = false
    renders_layers.layers[3].start_render = false

    renders_layers:set()
    
end


function START()
    
	print("core iniciando")
    set_res()

    initialize_render_layers()
    objects_layesrs:create()
    fps_count = create_fps_counter(objects_layesrs.hud,2,"resources/Fonts/pixel_snas.json")
    
    
    sceanes_db["test"]:load()
    
    
    
    
    
    
    
    
    

end



function UPDATE()
    fps_count:update_fps_cout()
    
end

function COLIDE(Colision)
end

function END()
end






