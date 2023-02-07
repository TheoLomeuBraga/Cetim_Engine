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

require("TMP_libs.material")



camera = nil
map_info = nil

objects_layesrs = layers_table:new_2D()


fps_counter = nil


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




function setup_objects()
    objects_layesrs:create()

    --hud
    fps_counter_material = matreial:new()
    fps_counter_material.shader = "resources/Shaders/text"
    fps_counter = create_text(objects_layesrs.hud, true, Vec3:new(-0.9, 0.9, 0),Vec3:new(0.0, 0.0, 0.0),Vec3:new(0.05, 0.05, 0.05), fps_counter_material, 2, "", "resources/Fonts/pixel_snas.json")

    --camera
    camera = create_camera_ortho(objects_layesrs.camera,Vec3:new(-1, 0, 0),Vec3:new(0, 0, 0),20,20,720,720,0.1,100)
    set_lisener_object(camera.object_ptr)
    
    --cenary
    tile_map_material = material:new()
    tile_map_material.shader = "resources/Shaders/sprite"
    map_info = load_2D_map(objects_layesrs.cenary,Vec3:new(0,0,0),Vec3:new(0,90,0),Vec3:new(0.1,0.1,0.1),"resources/Leveis 2D/tilemaps/tilemap.json","resources/Leveis 2D/tilesets/tileset.json","resources/Leveis 2D/tilesets",tile_map_material)
    map_info.map_object.components[components.render_tile_map].render_tilemap_only_layer = -1
    map_info.map_object.components[components.render_tile_map]:set()
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
    window.res.x = 720
    window.res.y = 720
    window:set()

    initialize_render_layers()
    
    setup_objects()
    
    
    
    
    
    
    

end



function UPDATE()
    
    
end

function COLIDE(Colision)
end

function END()
end






