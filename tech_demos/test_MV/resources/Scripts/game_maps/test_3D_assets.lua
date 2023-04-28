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
    this_map.background = create_render_shader(this_map.objects_layesrs.background_image, false, Vec3:new(0, 0, 0),
        Vec3:new(0, 0, 0), Vec3:new(1, 1, 1), 1, background_material)
end

function count_parent_objects(object_3D)
    ret = 0
    for index, value in ipairs(object_3D.children) do
        ret = ret + 1
        ret = ret + count_parent_objects(value)
    end
    return ret
end

function object_3D_to_game_object(father, render_layer, object_3D)
    local ret = {}


    local mesh_mat_size = math.min(tablelength(object_3D.meshes), tablelength(object_3D.materials))
    if mesh_mat_size > 0 then

        ret = create_mesh(father, false, deepcopyjson(object_3D.position), deepcopyjson(object_3D.rotation), deepcopyjson(object_3D.scale), render_layer, object_3D.materials,object_3D.meshes)

    else
        ret = game_object:new(father)
        ret:add_component(components.transform)
        ret.components[components.transform].is_ui = false
        ret.components[components.transform].position = deepcopyjson(object_3D.position)
        ret.components[components.transform].rotation = deepcopyjson(object_3D.rotation)
        ret.components[components.transform].scale = deepcopyjson(object_3D.scale)
        ret.components[components.transform]:set()
    end

    local obj_script = object_3D.variables.Script
    print(obj_script)

    for index, value in ipairs(object_3D.children) do
        object_3D_to_game_object(ret.object_ptr, render_layer, value)
    end

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
    this_map.camera = create_camera_perspective(this_map.objects_layesrs.camera, Vec3:new(-20, 0, 0), Vec3:new(0, 0, 0),90, 0.1, 100)
    set_lisener_object(this_map.camera.object_ptr)

    --local scene_3D = get_scene_3D("resources/3D Models/cube.gltf")
    local scene_3D = get_scene_3D("resources/3D Models/test_custom_proprietys.gltf")
    print("scene_3D")
    this_map.map = object_3D_to_game_object(this_map.objects_layesrs.cenary, 2, scene_3D.objects)
    this_map.map.components[components.transform].position = {x=0,y=-5,z=0}
    this_map.map.components[components.transform].rotation = {x=0,y=0,z=0}
    this_map.map.components[components.transform].scale = {x=1,y=1,z=1}
    this_map.map.components[components.transform]:set()

    
    --print("this_object_ptr {")   
    --local this_object = game_object:new(this_object_ptr) 
    --this_object:recreate()
    --deepprint(this_object)
    --print("}")
    
    --remove_object(this_map.map.object_ptr)


    --[[
    local mat = matreial:new()
    mat.shader = "resources/Shaders/mesh"
    mat.color = { r = 0, g = 0, b = 1, a = 1 }
    mat.textures = { "resources/Textures/white.png" }
    local cube_mesh = mesh_location:new("resources/3D Models/cube.gltf", "Cube")
    test_obj = create_mesh(this_map.objects_layesrs.cenary, false, Vec3:new(0, 1, 0), Vec3:new(0, 90, 0), Vec3:new(2, 2, 2), 2, { mat },{ cube_mesh })

    mat.color = { r = 1, g = 0, b = 1, a = 1 }
    test_obj_2 = create_mesh(test_obj.object_ptr, false, Vec3:new(4, 0, 0), Vec3:new(0, 0, 0), Vec3:new(1, 2, 1), 2, { mat },{ cube_mesh })

    mat.color = { r = 1, g = 0, b = 1, a = 1 }
    create_mesh(test_obj_2.object_ptr, false, Vec3:new(4, 0, 0), Vec3:new(0, 0, 0), Vec3:new(1, 2, 1), 2, { mat },{ cube_mesh })
    ]]
    

    

    --[[
    print("scene_3D {")
    tableprint(scene_3D)
    print("}")
    ]]
    --[[
    print("scene_3D.objects {")
    tableprint(scene_3D.objects.children[1])
    print("}")
    ]]
    --print("count_parent_objects: ", count_parent_objects(scene_3D.objects))


end

function test_3D_assets:unload()
    print("unloading sceane")
    this_map.objects_layesrs:destroy()
    clear_memory()
end

return test_3D_assets
