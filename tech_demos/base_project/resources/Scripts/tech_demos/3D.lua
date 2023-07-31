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
ceane_object_list = {}

function create_player_part(layer,obj_data)
end
function create_player_ceane(layer,ceane_data)
end


function create_game_object(father,obj_data)

    local ret = game_object:new(create_object(father))

    ret:add_component(components.transform)
    ret.components[components.transform].position = deepcopy(obj_data.position)
    ret.components[components.transform].rotation = deepcopy(obj_data.rotation)
    if obj_data.scale.x == 0 and obj_data.scale.x == 0 and obj_data.scale.x == 0 then
        ret.components[components.transform].scale = Vec3:new(1,1,1)
    else
        ret.components[components.transform].scale = deepcopy(obj_data.scale)
    end
    ret.components[components.transform]:set()

    local add_mesh = function ()
        if obj_data.meshes ~= nil and obj_data.materials ~= nil then
            ret:add_component(components.render_mesh)
            ret.components[components.render_mesh].layer = 2
            ret.components[components.render_mesh].meshes_cout = math.min(tablelength(obj_data.meshes),tablelength(obj_data.materials))
            ret.components[components.render_mesh].meshes = deepcopy(obj_data.meshes)
            ret.components[components.render_mesh].materials = deepcopy(obj_data.materials)
            ret.components[components.render_mesh]:set()
        end
    end



    local type = obj_data.variables.type
    if type == nil then
        add_mesh()
    elseif type == "rb" then
        add_mesh()
    elseif type == "sb" then
        add_mesh()
    elseif type == "music" then
        add_mesh()
        ret:add_component(components.audio_source)
        ret.components[components.audio_source].path = "resources/Audio/music/" .. obj_data.variables.sound_source ..".wav"
        ret.components[components.audio_source].loop = true
        ret.components[components.audio_source].volume = 5
        ret.components[components.audio_source].min_distance = 5
        ret.components[components.audio_source].atenuation = 1
        ret.components[components.audio_source]:set()
    elseif type == "player_start" then
        create_player_ceane(father,get_scene_3D("resources/Levels/3D/test_charter.gltf"))
    end

    for key, value in pairs(obj_data.children) do
        --create_game_object(father,value)
        create_game_object(ret.object_ptr,value)
    end

    ceane_object_list[obj_data.id] = deepcopy(ret)

    return ret

end
function create_ceane(father,ceane_data)
    return create_game_object(father,ceane_data.objects)
end

function demo:START(layers)

    this_layers = deepcopy(layers)

    demo.map_data = get_scene_3D("resources/Levels/3D/test_level.gltf")
    demo.map_objects = create_ceane(layers.cenary,demo.map_data)
    demo.map_objects.components[components.transform].position = Vec3:new(0,0,0)
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