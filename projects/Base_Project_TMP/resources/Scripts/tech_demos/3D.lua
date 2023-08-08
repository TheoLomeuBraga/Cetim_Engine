require("TMP_libs.short_cuts.create_mesh")
require("TMP_libs.short_cuts.create_collision")
require("TMP_libs.short_cuts.create_sound")
require("TMP_libs.objects.scene_3D")

local demo = {}

local demo = {
    map_data = {},
    map_objects = {},

    player_data = {},
    player_object = {}
}

local this_layers = {}

ceane_object_list = {}

local charter_object_list = {}
function create_player_part(father, obj_data)
    local ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].position = deepcopy(obj_data.position)
    ret.components[components.transform].rotation = deepcopy(obj_data.rotation)
    ret.components[components.transform].scale = deepcopy(obj_data.scale)
    ret.components[components.transform]:set()

    if obj_data.meshes ~= nil and obj_data.materials ~= nil then
        for key, value in pairs(obj_data.materials) do
            value.color = { r = 1, g = 0, b = 0, a = 1 }
            obj_data.materials[key] = deepcopy(value)
        end

        ret:add_component(components.render_mesh)
        ret.components[components.render_mesh].layer = 2
        ret.components[components.render_mesh].meshes_cout = math.min(tablelength(obj_data.meshes),
            tablelength(obj_data.materials))
        ret.components[components.render_mesh].meshes = deepcopy(obj_data.meshes)
        ret.components[components.render_mesh].materials = deepcopy(obj_data.materials)
        ret.components[components.render_mesh]:set()
    end

    

    charter_object_list[obj_data.id] = deepcopy(ret)

    for key, value in pairs(obj_data.children) do
        create_player_part(ret.object_ptr, value)
    end

    return ret
end

function create_player(player_obj, ceane_data)
    player_obj.components[components.transform].scale = Vec3:new(1, 1, 1)
    player_obj.components[components.transform]:set()

    for key, value in pairs(ceane_data.animations) do
        --print("animation name", value.name)
        --print("animation duration", value.duration)
    end

    player_obj:add_component(components.physics_3D)
    player_obj.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
    player_obj.components[components.physics_3D].collision_shape = collision_shapes.capsule
    player_obj.components[components.physics_3D].scale = Vec3:new(0.5, 2, 1)
    player_obj.components[components.physics_3D].rotacionarX = false
    player_obj.components[components.physics_3D].rotacionarY = false
    player_obj.components[components.physics_3D].rotacionarZ = false
    player_obj.components[components.physics_3D].friction = 0
    player_obj.components[components.physics_3D]:set()
    

    local armature = create_player_part(player_obj.object_ptr, ceane_data.objects)
    armature.components[components.transform]:get()
    local position = deepcopy(armature.components[components.transform].position)
    position.y = position.y -(6.0 * 0.25)
    armature.components[components.transform].position = position
    armature.components[components.transform].scale = Vec3:new( 0.25, 0.25, 0.25)
    armature.components[components.transform]:set()

    local charter_object_list_ptr = {}
    for key, value in pairs(charter_object_list) do
        charter_object_list_ptr[key] = value.object_ptr
    end

    player_obj:add_component(components.lua_scripts)
    player_obj.components[components.lua_scripts]:add_script("game_scripts/charter_control")
    player_obj.components[components.lua_scripts]:set_variable("game_scripts/charter_control", "charter_type", "3D")
    player_obj.components[components.lua_scripts]:set_variable("game_scripts/charter_control", "layers", this_layers)
    player_obj.components[components.lua_scripts]:set_variable("game_scripts/charter_control", "charter_size",Vec3:new(0.5, 2, 0.5))
    player_obj.components[components.lua_scripts]:set_variable("game_scripts/charter_control", "armature_data",{ ceane_data = deepcopy(ceane_data), object_list_ptr = deepcopy(charter_object_list_ptr),object_list = {} })

    return deepcopy(player_obj)
end

function create_game_object(father, obj_data)
    local ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].position = deepcopy(obj_data.position)
    ret.components[components.transform].rotation = deepcopy(obj_data.rotation)
    if obj_data.scale.x == 0 and obj_data.scale.y == 0 and obj_data.scale.z == 0 then
        ret.components[components.transform].scale = Vec3:new(1, 1, 1)
    else
        ret.components[components.transform].scale = deepcopy(obj_data.scale)
    end
    ret.components[components.transform]:set()

    local add_mesh = function(color)
        if obj_data.meshes ~= nil and obj_data.materials ~= nil then
            if color ~= nil then
                for key, value in pairs(obj_data.materials) do
                    value.color = deepcopy(color)
                    obj_data.materials[key] = deepcopy(value)
                end
            end

            ret:add_component(components.render_mesh)
            ret.components[components.render_mesh].layer = 2
            ret.components[components.render_mesh].meshes_cout = math.min(tablelength(obj_data.meshes),
                tablelength(obj_data.materials))
            ret.components[components.render_mesh].meshes = deepcopy(obj_data.meshes)
            ret.components[components.render_mesh].materials = deepcopy(obj_data.materials)
            ret.components[components.render_mesh]:set()
        end
    end

    local add_physics = function(rb)
        if obj_data.meshes ~= nil and obj_data.meshes[1] ~= nil then
            ret:add_component(components.physics_3D)
            if rb then
                ret.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic
            else
                ret.components[components.physics_3D].boady_dynamic = boady_dynamics.static
            end
            ret.components[components.physics_3D].collision_shape = collision_shapes.convex
            ret.components[components.physics_3D].collision_mesh = deepcopyjson(obj_data.meshes[1])
            ret.components[components.physics_3D].triger = false
            ret.components[components.physics_3D].scale = deepcopyjson(obj_data.scale)
            ret.components[components.physics_3D].friction = 10
            ret.components[components.physics_3D]:set()
        end
    end



    local type = obj_data.variables.type
    if type == nil then
        add_mesh(nil)
    elseif type == "rb" then
        add_mesh({ r = 0, g = 1, b = 0, a = 1 })
        add_physics(true)
    elseif type == "sb" then
        add_mesh({ r = 0, g = 0, b = 1, a = 1 })
        add_physics(false)
    elseif type == "music" then
        add_mesh({ r = 1, g = 0, b = 1, a = 1 })
        ret:add_component(components.audio_source)
        ret.components[components.audio_source].path = "resources/Audio/music/" ..
            obj_data.variables.sound_source .. ".wav"
        ret.components[components.audio_source].loop = true
        ret.components[components.audio_source].volume = 5
        ret.components[components.audio_source].min_distance = 5
        ret.components[components.audio_source].atenuation = 1
        ret.components[components.audio_source]:set()
    elseif type == "player_start" then
        ret = create_player(ret, get_scene_3D("resources/Levels/3D/test_charter.gltf"))
    end

    ceane_object_list[obj_data.id] = deepcopy(ret)

    for key, value in pairs(obj_data.children) do
        --create_game_object(father,value)
        create_game_object(ret.object_ptr, value)
    end

    return ret
end

function create_ceane(father, ceane_data)
    return create_game_object(father, ceane_data.objects)
end

function demo:START(layers)
    this_layers = deepcopy(layers)

    demo.map_data = get_scene_3D("resources/Levels/3D/test_level.gltf")
    demo.map_objects = create_ceane(layers.cenary, demo.map_data)
    demo.map_objects.components[components.transform].position = Vec3:new(0, 0, 0)
    demo.map_objects.components[components.transform]:set()
end

function demo:UPDATE()

end

function demo:END()
    remove_object(demo.map_objects.object_ptr)
    --remove_object(demo.player_object.object_ptr)
    clear_memory()
end

return demo
