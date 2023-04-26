--short cuts
require("TMP_libs.short_cuts.load_2D_map")
require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_camera")
require("TMP_libs.short_cuts.create_collision_mesh")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.objects.post_processing")
require("TMP_libs.objects.window")
require("TMP_libs.objects.collision_shapes")
require("TMP_libs.objects.material")

require("TMP_libs.components.component_all")

require("TMP_libs.layers_table")



local this_sceane = {}
this_sceane.camera = nil
this_sceane.tile_map_info = nil
this_sceane.background = nil
this_sceane.objects_layesrs = nil


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


local test_3D_assets = {}

function test_3D_assets:load()
    initialize_render_settings()
    this_sceane.objects_layesrs = layers_table:new_2D()
    this_sceane.objects_layesrs:create()
    
    --create_audio(this_sceane.objects_layesrs.sound,"resources/Audio/teste de audio.wav",true,5)
    --background
    
    create_background("resources/Textures/fundo A.png")

    --camera
    --this_sceane.camera = create_camera_ortho(this_sceane.objects_layesrs.camera,Vec3:new(-10, 0, 0),Vec3:new(0, 0, 0),150,150,720,720,0.1,100)
    this_sceane.camera = create_camera_perspective(this_sceane.objects_layesrs.camera,Vec3:new(-10, 0, 0),Vec3:new(0, 0, 0),90,0.1,100)
    set_lisener_object(this_sceane.camera.object_ptr)

    mat = matreial:new()
    mat.shader = "resources/Shaders/mesh"
    mat.color = {r = 1,g = 0,b = 0,a = 1}       
    pos = Vec3:new(0,0,0)
    create_mesh(this_sceane.objects_layesrs.background_3D,false,pos,Vec3:new(0,-90,0),Vec3:new(1,1,1),2,{mat},{mesh_location:new("resources/3D Models/cube.gltf","Suzanne")})
end

function test_3D_assets:unload()
    print("unloading sceane")
    test_3D_assets.objects_layesrs:destroy()
    clear_memory()
end


return test_3D_assets