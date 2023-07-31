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
    ret.components[components.transform].scale = deepcopy(obj_data.scale)
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

    elseif type == "player_start" then
        create_player_ceane(layer,get_scene_3D("resources/Levels/3D/test_charter.gltf"))
    end

    --demo.player_data = get_scene_3D("resources/Levels/3D/test_charter.gltf")
    --demo.player_object = create_player_ceane(layers.cenary,demo.map_data)

    

    for key, value in pairs(obj_data.children) do
        create_game_object(father,value)
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
    demo.map_objects.components[components.transform]:change_position(0,-10,0)
    
    
end

function demo:UPDATE()

end

function demo:END()
    remove_object(demo.map_objects.object_ptr)
    --remove_object(demo.player_object.object_ptr)
    clear_memory()
end

return demo