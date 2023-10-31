require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_render_shader")

menu = {
    obj = nil,
    open = function()
        if menu.obj == nil then
            menu.obj = game_object:new(create_object(global_data:get_var("layers").hud))
            menu.obj:add_component(components.transform)
            menu.obj.components[components.transform]:set()
            menu.obj:add_component(components.lua_scripts)
            menu.obj.components[components.lua_scripts]:add_script("game_scripts/menus")
            menu.obj.components[components.lua_scripts]:set_variable("game_scripts/menus", "menu_selectred", "pause")
        end
    end,

    close = function(menu_obj)
        if menu.obj.object_ptr ~= nil then
            remove_object(menu.obj.object_ptr)
        end
        menu.obj = nil
    end,
}

loading_screen = {
    obj = nil,
    spin_obj = nil,
    cam = nil,

    open = function()
        loading_screen.obj = game_object:new(global_data:get_var("core_object_ptr"))
        loading_screen.obj.components[components.lua_scripts]:call_function("core", "set_load_image",
            { path = "resources/Textures/loading.png", color = { r = 1, g = 1, b = 1 } })
        loading_screen.cam = create_camera_perspective(layers.camera, { x = 0, y = 0, z = 0 }, { x = 0, y = 0, z = 0 },
            90, 0.1, 1000)
        set_lisener_object(loading_screen.cam.object_ptr)




        local mat = matreial:new()
        mat.shader = "resources/Shaders/sprite"
        mat.textures = { "resources/Textures/spiral.svg" }
        loading_screen.spin_obj = create_render_shader(global_data:get_var("layers").hud, true, Vec3:new(0.8, -0.8, 0),
            Vec3:new(0, 0, 0), Vec3:new(0.2, 0.2, 0.2), 5, mat)


        loading_screen.obj.components[components.lua_scripts]:add_script("game_scripts/loading_sreen_script")
        loading_screen.obj.components[components.lua_scripts]:set_variable("game_scripts/loading_sreen_script",
            "obj_ptr_to_rotate", loading_screen.spin_obj.object_ptr)
    end,

    close = function()
        loading_screen.obj.components[components.lua_scripts]:call_function("core", "set_load_image", {})
        loading_screen.obj.components[components.lua_scripts]:remove_script("game_scripts/loading_sreen_script")
        remove_object(loading_screen.cam.object_ptr)
        remove_object(loading_screen.spin_obj.object_ptr)
    end,
}

local entity_ptr_list = {}
local scene_ptr_list = {}

cenary_builders = {

    yield_count_down_total_time = 5,
    yield_count_down = 5,

    entity_part = function(father, layer, part_data, shader, use_oclusion, yield)
        local ret = game_object:new(create_object(father))
        entity_ptr_list[part_data.id] = ret.object_ptr

        ret:add_component(components.transform)
        ret.components[components.transform].position = deepcopy(part_data.position)
        ret.components[components.transform].rotation = deepcopy(part_data.rotation)

        if part_data.scale.x == 0 and part_data.scale.y == 0 and part_data.scale.z == 0 then
            ret.components[components.transform].scale = Vec3:new(1, 1, 1)
        else
            ret.components[components.transform].scale = deepcopy(part_data.scale)
        end

        ret.components[components.transform]:set()

        for key, value in pairs(part_data.materials) do
            part_data.materials[key].shader = shader
        end

        if part_data.meshes ~= nil and part_data.materials ~= nil then --and math.min(#part_data.meshes, #part_data.materials) > 0 then
            ret:add_component(components.render_mesh)
            ret.components[components.render_mesh].layer = layer
            ret.components[components.render_mesh].meshes_cout = math.min(#part_data.meshes, #part_data.materials)
            ret.components[components.render_mesh].meshes = part_data.meshes
            ret.components[components.render_mesh].materials = part_data.materials
            ret.components[components.render_mesh].use_oclusion = use_oclusion
            ret.components[components.render_mesh]:set()
        end




        if yield == true then
            cenary_builders.yield_count_down = cenary_builders.yield_count_down - 1
            if cenary_builders.yield_count_down < 1 then
                coroutine.yield()
                cenary_builders.yield_count_down = cenary_builders.yield_count_down_total_time
            end
        end

        for key, value in pairs(part_data.children) do
            cenary_builders.entity_part(ret.object_ptr, layer, value, shader, use_oclusion, yield)
        end

        return ret
    end,

    entity = function(father, layer, ceane_data, shader, use_oclusion, yield)
        local ret = {
            obj = {},
            parts_ptr_list = {},
            parts_list = {},
        }

        if yield == nil then yield = false end
        cenary_builders.yield_count_down = cenary_builders.yield_count_down_total_time
        local entity_parts = cenary_builders.entity_part(father, layer, ceane_data.objects, shader, use_oclusion, yield)

        ret.obj = deepcopy(entity_parts)
        ret.parts_ptr_list = deepcopy(entity_ptr_list)
        for key, value in pairs(ret.parts_ptr_list) do
            ret.parts_list[key] = game_object:new(value)
        end

        entity_ptr_list = {}
        return ret
    end,

    scene_poly_meshes = {},

    scene_part = function(father, layer, part_data, yield)
        local ret = {}
        ret = game_object:new(create_object(father))

        scene_ptr_list[part_data.id] = ret.object_ptr

        ret:add_component(components.transform)
        ret.components[components.transform].position = deepcopy(part_data.position)
        ret.components[components.transform].rotation = deepcopy(part_data.rotation)

        if part_data.scale.x == 0 and part_data.scale.y == 0 and part_data.scale.z == 0 then
            ret.components[components.transform].scale = Vec3:new(1, 1, 1)
        else
            ret.components[components.transform].scale = deepcopy(part_data.scale)
        end

        ret.components[components.transform]:set()

        local add_physics = function(rb, is_triger)
            if part_data.meshes ~= nil and part_data.meshes[1] ~= nil then
                ret:add_component(components.physics_3D)
                if rb then
                    ret.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
                    ret.components[components.physics_3D].rotate_x = false
                    ret.components[components.physics_3D].rotate_y = false
                    ret.components[components.physics_3D].rotate_z = false
                else
                    ret.components[components.physics_3D].boady_dynamic = boady_dynamics.static
                end

                if part_data.variables.collision_shape == nil then
                    ret.components[components.physics_3D].collision_shape = collision_shapes.convex
                    ret.components[components.physics_3D].collision_mesh = deepcopyjson(part_data.meshes[1])
                elseif part_data.variables.collision_shape == "cube" then
                    ret.components[components.physics_3D].collision_shape = collision_shapes.cube
                elseif part_data.variables.collision_shape == "sphere" then
                    ret.components[components.physics_3D].elasticity = 1
                    ret.components[components.physics_3D].collision_shape = collision_shapes.sphere
                end


                ret.components[components.physics_3D].triger = is_triger
                ret.components[components.physics_3D].scale = deepcopyjson(part_data.scale)
                ret.components[components.physics_3D].friction = 2
                ret.components[components.physics_3D]:set()
            end
        end

        local add_mesh = function(color)
            if part_data.meshes ~= nil and part_data.materials ~= nil then
                if color ~= nil then
                    for key, value in pairs(part_data.materials) do
                        part_data.materials[key].color = deepcopy(color)
                        part_data.materials[key] = deepcopy(value)
                    end
                end

                ret:add_component(components.render_mesh)
                ret.components[components.render_mesh].layer = layer
                ret.components[components.render_mesh].meshes_cout = math.min(#part_data.meshes, #part_data.materials)
                ret.components[components.render_mesh].meshes = deepcopy(part_data.meshes)
                ret.components[components.render_mesh].materials = deepcopy(part_data.materials)
                ret.components[components.render_mesh]:set()
            end
        end

        if part_data.variables.type == "test_poly_mesh" then
            ret:add_component(components.physics_3D)
            ret.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
            ret.components[components.physics_3D].collision_shape = collision_shapes.box
            ret.components[components.physics_3D].triger = false
            ret.components[components.physics_3D].scale = { x = 0.5, y = 1, z = 1 }
            ret.components[components.physics_3D].friction = 2
            ret.components[components.physics_3D].rotate_x = false
            ret.components[components.physics_3D].rotate_y = false
            ret.components[components.physics_3D].rotate_z = false
            ret.components[components.physics_3D]:set()

            table.insert(cenary_builders.scene_poly_meshes.components[components.render_poly_mesh].objects,
                ret.object_ptr)
        elseif part_data.variables.type == "sb" then
            add_physics(false, false)
            add_mesh(nil)
        elseif part_data.variables.type == "rb" then
            add_physics(true, false)
            add_mesh(nil)
        elseif part_data.variables.type == "camera" then
            ret:add_component(components.camera)
            ret.components[components.camera]:set()
        elseif part_data.variables.type == "player_start" then
            ret:add_component(components.lua_scripts)
            ret:add_component(components.physics_3D)
            ret.components[components.lua_scripts]:add_script("game_scripts/charter")
            ret.components[components.lua_scripts]:add_script("game_scripts/charter_arcenal")
            ret.components[components.lua_scripts]:add_script("game_scripts/charter_interaction")
        elseif part_data.variables.type == "music" then
            ret:add_component(components.audio_source)
            ret.components[components.audio_source].path = "resources/Audio/musics/" ..
                part_data.variables.sound_source .. ".wav"
            ret.components[components.audio_source].loop = true
            ret.components[components.audio_source].volume = 5
            ret.components[components.audio_source].min_distance = 5
            ret.components[components.audio_source].atenuation = 1
            ret.components[components.audio_source]:set()

            add_mesh(nil)
        elseif part_data.variables.type == "sound" then
            ret:add_component(components.audio_source)
            ret.components[components.audio_source].path = "resources/Audio/sounds/" ..
                part_data.variables.sound_source .. ".wav"
            ret.components[components.audio_source].loop = true
            ret.components[components.audio_source].volume = 5
            ret.components[components.audio_source].min_distance = 5
            ret.components[components.audio_source].atenuation = 1
            ret.components[components.audio_source]:set()

            add_mesh(nil)
        elseif part_data.variables.type == "mensage" then
            ret:add_component(components.lua_scripts)
            ret.components[components.lua_scripts]:add_script("game_scripts/mensage")

            local mensage = part_data.variables.mensage
            local mensage_index = part_data.variables.mensage_index
            if mensage_index ~= nil then
                ret.components[components.lua_scripts]:set_variable("game_scripts/mensage", "mensage_index",
                    mensage_index)
            elseif mensage ~= nil then
                ret.components[components.lua_scripts]:set_variable("game_scripts/mensage", "mensage", mensage)
            end

            add_mesh(nil)
            add_physics(false, false)
        elseif part_data.variables.type == "passage" then
            ret:add_component(components.lua_scripts)
            ret.components[components.lua_scripts]:add_script("game_scripts/passage")
            ret.components[components.lua_scripts]:set_variable("game_scripts/passage", "passage_target",
                part_data.variables.passage_target)

            add_mesh(nil)
            add_physics(false, true)
        elseif part_data.variables.type == "door_triger" then
            ret:add_component(components.lua_scripts)
            ret.components[components.lua_scripts]:add_script("game_scripts/door_triger")
            ret.components[components.lua_scripts]:set_variable("game_scripts/door_triger", "triger_target",
                part_data.variables.triger_target)
            add_mesh(nil)
            add_physics(false, true)
        elseif part_data.variables.type == nil then
            add_mesh(nil)
        end

        if yield == true then
            cenary_builders.yield_count_down = cenary_builders.yield_count_down - 1
            if cenary_builders.yield_count_down < 0 then
                coroutine.yield()
                cenary_builders.yield_count_down = cenary_builders.yield_count_down_total_time
            end
        end

        for key, value in pairs(part_data.children) do
            cenary_builders.scene_part(ret.object_ptr, layer, value, yield)
        end

        return ret
    end,

    scene = function(father, layer, ceane_data, yield)
        local ret = {
            obj = {},
            parts_ptr_list = {},
            parts_list = {},
        }

        local mat = matreial:new()
        mat.shader = "resources/Shaders/mesh"
        mat.textures = { "resources/Textures/null.png" }
        mat.color = { r = 0.5, g = 0.5, b = 1, a = 1 }

        cenary_builders.scene_poly_meshes = game_object:new(create_object(father))
        cenary_builders.scene_poly_meshes:add_component(components.render_poly_mesh)
        cenary_builders.scene_poly_meshes.components[components.render_poly_mesh].layer = layer
        cenary_builders.scene_poly_meshes.components[components.render_poly_mesh].meshes_cout = 1
        cenary_builders.scene_poly_meshes.components[components.render_poly_mesh].meshes = deepcopy({
            mesh_location:new("resources/Levels/3D/hub/hub.gltf", "Suzanne") })
        cenary_builders.scene_poly_meshes.components[components.render_poly_mesh].materials = { mat }


        --if yield == nil then yield = false end
        cenary_builders.yield_count_down = cenary_builders.yield_count_down_total_time
        ret.obj = deepcopy(cenary_builders.scene_part(father, layer, ceane_data.objects, yield))
        ret.parts_ptr_list = deepcopy(scene_ptr_list)
        for key, value in pairs(ret.parts_ptr_list) do
            ret.parts_list[key] = game_object:new(value)
        end


        ret.obj:add_component(components.lua_scripts)
        ret.obj.components[components.lua_scripts]:add_script("game_scripts/door_manager")
        ret.obj.components[components.lua_scripts]:set_variable("game_scripts/door_manager", "map_file", ceane_data.path)
        ret.obj.components[components.lua_scripts]:set_variable("game_scripts/door_manager", "parts_ptr_list",
            ret.parts_ptr_list)



        cenary_builders.scene_poly_meshes.components[components.render_poly_mesh]:set()
        cenary_builders.scene_poly_meshes = {}

        return ret
    end,

}
