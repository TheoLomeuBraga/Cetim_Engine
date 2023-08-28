require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")
require("engine_libs.objects.global_data")
require("engine_libs.objects.time")
require("engine_libs.short_cuts.create_sound")

menu = {
    obj = nil,
    open = function ()
        if menu.obj == nil then
            menu.obj = game_object:new(create_object(global_data:get_var("layers").hud))
            menu.obj:add_component(components.transform)
            menu.obj.components[components.transform]:set()
            menu.obj:add_component(components.lua_scripts)
            menu.obj.components[components.lua_scripts]:add_script("game_scripts/menus")
            menu.obj.components[components.lua_scripts]:set_variable("game_scripts/menus", "menu_selectred", "pause")
        end
    end,

    close = function (menu_obj)
        if menu.obj.object_ptr ~= nil then
            remove_object(menu.obj.object_ptr)
        end
        menu.obj = nil
    end,
}

cenary_builders = {

    yield_count_down_total_time = 10,
    yield_count_down = 10,

    entity_part = function (father, part_data)
    end,
    entity = function (entity_obj, ceane_data)
    end,

    is_entity = function (part_data)
        entyty_types = {"player_start","camera","music","sound"}
        --[[
        for index, value in ipairs(entyty_types) do
            if part_data.variables.type == value then
                return true
            end
        end
        ]]
        
        return false
    end,

    scene_part = function (cenary_father,entities_father, part_data,yield)
        local ret = {}
        if cenary_builders.is_entity(part_data) then
            ret = game_object:new(create_object(entities_father))
        else
            ret = game_object:new(create_object(cenary_father))
        end
        

        ret:add_component(components.transform)
        ret.components[components.transform].position = deepcopy(part_data.position)
        ret.components[components.transform].rotation = deepcopy(part_data.rotation)

        if part_data.scale.x == 0 and part_data.scale.y == 0 and part_data.scale.z == 0 then
            ret.components[components.transform].scale = Vec3:new(1, 1, 1)
        else
            ret.components[components.transform].scale = deepcopy(part_data.scale)
        end
        
        ret.components[components.transform]:set()

        local add_physics = function(rb,is_triger)
            if part_data.meshes ~= nil and part_data.meshes[1] ~= nil then
                ret:add_component(components.physics_3D)
                if rb then
                    ret.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
                else
                    ret.components[components.physics_3D].boady_dynamic = boady_dynamics.static
                end
                ret.components[components.physics_3D].collision_shape = collision_shapes.convex
                ret.components[components.physics_3D].collision_mesh = deepcopyjson(part_data.meshes[1])
                ret.components[components.physics_3D].triger = is_triger
                ret.components[components.physics_3D].scale = deepcopyjson(part_data.scale)
                ret.components[components.physics_3D].friction = 10
                ret.components[components.physics_3D]:set()
            end
        end

        local add_mesh = function(color)
            if part_data.meshes ~= nil and part_data.materials ~= nil then
                if color ~= nil then
                    for key, value in pairs(part_data.materials) do
                        value.color = deepcopy(color)
                        part_data.materials[key] = deepcopy(value)
                    end
                end
    
                ret:add_component(components.render_mesh)
                ret.components[components.render_mesh].layer = 2
                ret.components[components.render_mesh].meshes_cout = math.min(tablelength(part_data.meshes),tablelength(part_data.materials))
                ret.components[components.render_mesh].meshes = deepcopy(part_data.meshes)
                ret.components[components.render_mesh].materials = deepcopy(part_data.materials)
                ret.components[components.render_mesh]:set()
            end
        end

        

        if part_data.variables.type == "sb" then

            add_physics(false,false)
            add_mesh(nil)

        elseif part_data.variables.type == "rb" then

            add_physics(true,false)
            add_mesh(nil)

        elseif part_data.variables.type == "camera" then

            ret:add_component(components.camera)
            ret.components[components.camera]:set()

        elseif part_data.variables.type == "player_start" then

            ret:add_component(components.lua_scripts)
            ret:add_component(components.physics_3D)
            ret.components[components.lua_scripts]:add_script("game_scripts/hmtf_charter")

        elseif part_data.variables.type == "music" then

            ret:add_component(components.audio_source)
            ret.components[components.audio_source].path = "resources/Audio/musics/" .. part_data.variables.sound_source .. ".wav"
            ret.components[components.audio_source].loop = true
            ret.components[components.audio_source].volume = 5
            ret.components[components.audio_source].min_distance = 5
            ret.components[components.audio_source].atenuation = 1
            ret.components[components.audio_source]:set()

            add_mesh(nil)

        elseif part_data.variables.type == "sound" then

            ret:add_component(components.audio_source)
            ret.components[components.audio_source].path = "resources/Audio/sounds/" .. part_data.variables.sound_source .. ".wav"
            ret.components[components.audio_source].loop = true
            ret.components[components.audio_source].volume = 5
            ret.components[components.audio_source].min_distance = 5
            ret.components[components.audio_source].atenuation = 1
            ret.components[components.audio_source]:set()

            add_mesh(nil)

        elseif part_data.variables.type == "mensage" then

            local mensage = part_data.variables.mensage

            ret:add_component(components.lua_scripts)
            ret.components[components.lua_scripts]:add_script("game_scripts/mensage")
            ret.components[components.lua_scripts]:set_variable("game_scripts/mensage","mensage",mensage)

            add_mesh(nil)
            add_physics(false,false)

        elseif part_data.variables.type == "passage" then

            ret:add_component(components.lua_scripts)
            ret.components[components.lua_scripts]:add_script("game_scripts/passage")
            ret.components[components.lua_scripts]:set_variable("game_scripts/passage","passage_target",part_data.variables.passage_target)

            add_mesh(nil)
            add_physics(false,true)

        elseif part_data.variables.type == nil then
            add_mesh(nil)
        end

        

        if yield == true then

            cenary_builders.yield_count_down = cenary_builders.yield_count_down - 1
            if cenary_builders.yield_count_down < 1 then
                coroutine.yield()
                cenary_builders.yield_count_down = cenary_builders.yield_count_down_total_time
            end
            
        end
        
        

        for key, value in pairs(part_data.children) do
            cenary_builders.scene_part(ret.object_ptr,entities_father, value)
        end
        
        return ret
    end,
    scene = function (cenary_father,entities_father, ceane_data,yield)
        if yield == nil then yield = false end
        cenary_builders.yield_count_down = cenary_builders.yield_count_down_total_time
        
        return cenary_builders.scene_part(cenary_father,entities_father, ceane_data.objects,yield)
    end,
}