-- short cuts
require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_camera")
require("TMP_libs.short_cuts.create_collision_mesh")
require("TMP_libs.objects.post_processing")
require("TMP_libs.objects.window")
require("TMP_libs.objects.collision_shapes")
require("TMP_libs.objects.input")
require("TMP_libs.objects.global_data")

require("TMP_libs.components.component_all")
require("TMP_libs.layers_table")
require("TMP_libs.objects.scene_3D")

require("math")

assets_from_map = require("game_maps.test_level.assets")

local test_3D_game = {}

function test_3D_game:initialize_render_settings()
    window.resolution.x = 640
    window.resolution.y = 480
    window:set()

    renders_layers.layers_size = 4

    renders_layers.layers[1] = render_layer:new()
    renders_layers.layers[1].end_render = false
    renders_layers.layers[1].use_deep = true
    renders_layers.layers[1].clean_color = false
    renders_layers.layers[1].start_render = true

    renders_layers.layers[2] = render_layer:new()
    renders_layers.layers[2].end_render = false
    renders_layers.layers[2].use_deep = true
    renders_layers.layers[2].clean_color = false
    renders_layers.layers[2].start_render = false

    renders_layers.layers[3] = render_layer:new()
    renders_layers.layers[3].end_render = true
    renders_layers.layers[3].use_deep = true
    renders_layers.layers[3].clean_color = false
    renders_layers.layers[3].start_render = false

    renders_layers:set()

    post_processing:get()
    post_processing.material.shader = "resources/Shaders/post_processing"
    post_processing:set()
end

function test_3D_game:create_background()
    background_material = material:new()
    background_material.shader = "resources/Shaders/background"
    background_material.textures = {"resources/Textures/white.png"}
    background_material.color = {
        r = 0.25,
        g = 0.25,
        b = 0.25,
        a = 1
    }
    self.objects_layesrs.background_image = create_render_shader(self.objects_layesrs.background_image, false,
        Vec3:new(0, 0, 0), Vec3:new(0, 0, 0), Vec3:new(1, 1, 1), 1, background_material)
end

function test_3D_game:create_test_camera()
    self.camera = assets_from_map.free_camera(self.objects_layesrs.camera, Vec3:new(0, 20, 0), Vec3:new(45, 0, 0))
end

local game_objects_in_cene_ret = {}
local texture_dictionary = require("game_maps.test_level.materials")
function test_3D_game:object_3D_to_game_object(father, render_layer, object_3D)

    local ret = {}

    local object_type = object_3D.variables.type
    

    -- print("name",object_3D.name)

    if tablelength(object_3D.meshes) > 0 then

        -- local mesh_mat_size = math.min(tablelength(object_3D.meshes), tablelength(object_3D.materials))
        local materials = {}
        local i = 1
        for i, v in ipairs(object_3D.materials) do
            materials[i] = texture_dictionary(object_type)
            i = i + 1
        end

        -- print(object_3D.name, object_3D.position.x, object_3D.position.y, object_3D.position.x)

        ret = create_mesh(father, false, deepcopyjson(object_3D.position), deepcopyjson(object_3D.rotation),
            deepcopyjson(object_3D.scale), render_layer, materials, object_3D.meshes)

        ret.name = object_3D.name

        if object_type == "test_rb" then
            ret:add_component(components.physics_3D)
            ret.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic

            ret.components[components.physics_3D].collision_shape = collision_shapes.convex
            ret.components[components.physics_3D].collision_mesh = object_3D.meshes[1]

            ret.components[components.physics_3D]:set()
        elseif object_type == "test_sb" then
            ret:add_component(components.physics_3D)
            ret.components[components.physics_3D].boady_dynamic = boady_dynamics.static

            ret.components[components.physics_3D].collision_shape = collision_shapes.convex
            ret.components[components.physics_3D].collision_mesh = object_3D.meshes[1]

            ret.components[components.physics_3D]:set()
        end

    else
        if object_type == nil then
            ret = game_object:new(create_object(father))
            ret.name = object_3D.name
            ret:add_component(components.transform)
            ret.components[components.transform].position = deepcopyjson(object_3D.position)
            ret.components[components.transform].rotation = deepcopyjson(object_3D.rotation)
            ret.components[components.transform].scale = deepcopyjson(object_3D.scale)
            ret.components[components.transform]:set()
        elseif object_type == "player_start" then
            print("player_start")
            test_3D_game.camera = assets_from_map.free_camera(test_3D_game.objects_layesrs.camera, deepcopyjson(object_3D.position), deepcopyjson(object_3D.rotation))
        end

    end

    -- print(object_3D.name,"tablelength(object_3D.children)",tablelength(object_3D.children))

    for index, value in ipairs(object_3D.children) do
        ret.children[index] = self:object_3D_to_game_object(ret.object_ptr, render_layer, value)[1]
    end

    
    game_objects_in_cene_ret[object_3D.id] = deepcopy(ret)

    return {ret,deepcopy(game_objects_in_cene_ret)}
end

function test_3D_game:load_assets(path)
    local scene_3D = get_scene_3D(path)

    --print("scene_3D.animations", tablelength( scene_3D.animations))
    --print("scene_3D.animations[1].key_frames", tablelength( scene_3D.animations[1].key_frames))

    game_objects_in_cene_ret = {}
    local ret = self:object_3D_to_game_object(self.objects_layesrs.cenary, 2, scene_3D.objects)
    ret[1].components[components.transform].position = Vec3:new(0, 0, 0)
    ret[1].components[components.transform].rotation = Vec3:new(0, 0, 0)
    ret[1].components[components.transform].scale = Vec3:new(1, 1, 1)
    ret[1].components[components.transform]:set()

    --print("AAAAA")
    apply_key_frame(ret[2],scene_3D.animations[1].key_frames[50])

    return ret
end

function test_3D_game:load()
    self:initialize_render_settings()
    self.objects_layesrs = layers_table:new_3D()
    self.objects_layesrs:create()

    self:create_background()
    --self:create_test_camera()

    -- self.assets = self:load_assets("resources/3D Models/test_collision.gltf")
    -- self.assets = self:load_assets("resources/3D Models/guns.gltf")
    self.assets = self:load_assets("resources/3D Models/test_level.gltf")

end

local R_last_frame = false
function test_3D_game:update()
    if R_last_frame == true and keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.r]) ==
        0 then
        local core = game_object:new(global_data:get_var("core_object_ptr"))
        core.components[components.lua_scripts]:call_function("core", "load_map_from_other", {"test"})
    end
    R_last_frame = keys_axis:get_input(input_devices.keyboard, input_keys.keyboard[input_keys.keyboard.r]) == 1
end

function test_3D_game:unload()
    self.objects_layesrs:destroy()
    clear_memory()
end

return test_3D_game
