--short cuts
require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_camera")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.objects.post_processing")
require("TMP_libs.objects.window")
require("TMP_libs.components.component_all")
require("TMP_libs.layers_table")

function initialize_render_settings()
    
    window.resolution.x = 720
    window.resolution.y = 720
    window:set()
    

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

    post_processing:get()
    post_processing.material.shader = "resources/Shaders/post_processing"
    post_processing:set()
    
end

local this_map = {}

function create_background()
    background_material = material:new()
    background_material.shader = "resources/Shaders/background"
    background_material.textures[1] = "resources/Textures/white.png"
    background_material.color = {r = 0, g = 0,b = 0, a = 1}
    this_map.background = create_render_shader(this_map.objects_layesrs.background_image,false,Vec3:new(0, 0, 0),Vec3:new(0, 0, 0),Vec3:new(1, 1, 1),1,background_material)
end





local test_selection = {}

terminal = {}

function test_selection:load()
    initialize_render_settings()
    this_map.objects_layesrs = layers_table:new_2D()
    this_map.objects_layesrs:create()
    create_background()
    this_map.camera = create_camera_perspective(this_map.objects_layesrs.camera,Vec3:new(-10, 0, 0),Vec3:new(0, 0, 0),90,0.1,100)

    mat = material:new()
    mat.shader = "resources/Shaders/text"
    mat.color = {r = 0, g = 1,b = 0, a = 1}
    terminal = create_text(this_map.objects_layesrs.hud,true,Vec3:new(-0.9,0.9,0.9),Vec3:new(0,0,0),Vec3:new(0.05,0.05,0.05),mat,3,"hello world","resources/Fonts/Glowworm Regular.json")
end

function test_selection:unload()
    print("unloading sceane")
    this_map.objects_layesrs:destroy()
    clear_memory()
end

return test_selection