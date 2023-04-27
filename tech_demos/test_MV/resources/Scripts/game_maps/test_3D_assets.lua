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


require("TMP_libs.components.component_all")
require("TMP_libs.layers_table")
require("TMP_libs.objects.scene_3D")

require("math")

local this_map = {}



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

function create_background(image)
    background_material = material:new()
    background_material.shader = "resources/Shaders/background"
    background_material.textures[1] = "resources/Textures/fundo A.png"
    this_map.background = create_render_shader(this_map.objects_layesrs.background_image,false,Vec3:new(0, 0, 0),Vec3:new(0, 0, 0),Vec3:new(1, 1, 1),1,background_material)
end

function count_parent_objects(object_3D)
    ret = 0
    for index, value in ipairs(object_3D.children) do
        ret = ret + 1
        ret = ret + count_parent_objects(value)
    end
    return ret
end

function object_3D_to_game_object(father,render_layer,object_3D)
    ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].is_ui = false
    ret.components[components.transform].position = deepcopy(object_3D.position)
    ret.components[components.transform].rotation = deepcopy(object_3D.rotation)
    ret.components[components.transform].scale = deepcopy(object_3D.scale)
    ret.components[components.transform]:set()

    
    local mesh_mat_size = math.min(tablelength(object_3D.meshes),tablelength(object_3D.materials))
    if mesh_mat_size > 0 then
        ret:add_component(components.render_mesh)
        ret.components[components.render_mesh].layer = render_layer
        ret.components[components.render_mesh].meshes_cout = mesh_mat_size

        ret.components[components.render_mesh].meshes = object_3D.meshes
        ret.components[components.render_mesh].materials = object_3D.materials

        ret.components[components.render_mesh]:set()
    end
    
    
    for index, value in ipairs(object_3D.children) do
        object_3D_to_game_object(ret.object_ptr,render_layer,value)
    end
    
    return ret
end

function create_sceane_3D_with_tf(father,pos,rot,sca,sceane_3D)
    ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.transform].scale = deepcopy(sca)
    ret.components[components.transform]:set()

    object_3D_to_game_object(ret.object_ptr,2,scene_3D.objects)
    return ret
end

function create_sceane_3D(father,sceane_3D)

    object_3D_to_game_object(father,2,scene_3D.objects)
    return ret
end

local test_3D_assets = {}

function test_3D_assets:load()
    this_map = {}
    print("loading test map")
    initialize_render_settings()
    this_map.objects_layesrs = layers_table:new_3D()
    this_map.objects_layesrs:create()
    
    create_background()

    --camera
    this_map.camera = create_camera_perspective(this_map.objects_layesrs.camera,Vec3:new(-10, 0, 0),Vec3:new(0, 0, 0),90,0.1,100)
    set_lisener_object(this_map.camera.object_ptr)

    scene_3D = get_scene_3D("resources/3D Models/cube.gltf")
    --object_3D_to_game_object(this_map.objects_layesrs.cenary,2,scene_3D.objects)
    pos = {x = 0,y = 0,z = 0}
    rot = {x = 0,y = 0,z = 0}
    sca = {x = 1,y = 1,z = 1}
    create_sceane_3D_with_tf(this_map.objects_layesrs,pos,rot,sca,scene_3D)

    --deepprint(scene_3D)
    print("scene_3D {")
    tableprint(scene_3D)
    print("}")

    print("scene_3D.objects {")
    tableprint(scene_3D.objects)
    print("}")

    print("count_parent_objects: ",count_parent_objects(scene_3D.objects))

end

function test_3D_assets:unload()
    print("unloading sceane")
    this_map.objects_layesrs:destroy()
    clear_memory()
end

return test_3D_assets