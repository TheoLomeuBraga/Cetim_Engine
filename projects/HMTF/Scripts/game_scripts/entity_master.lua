require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_mesh")
require("short_cuts.create_collision")
require("game_scripts.resources.playable_scene")
require("math")


entitys_list = {}


player_position = { x = 0, y = 0, z = 0 }

--[[
    local pre_calculated_paths_ret = {
        path = {},
        progression = 0
    }
]]

function distance(a, b)
    local dx = b.x - a.x
    local dy = b.y - a.y
    local dz = b.z - a.z
    return math.sqrt(dx * dx + dy * dy + dz * dz)
end

time_to_clean_paths = 0
function clean_pre_calculated_paths()
    for index, value in ipairs(entitys_list) do
        value.path = nil
        value.progression = nil
    end
end

function START()
end

whait_time = 0
local update_entity_map = {
    test_entity = function(entity)
        local pos = entity.obj.components.transform:get_global_position()

        if entity.path == nil or #entity.path == 0 or entity.progression == nil or #entity.progression == 0 then
            entity.path = generate_navmesh_short_path(pos, player_position)
            entity.progression = { 0.0 }
        end
        

        if distance(pos, player_position) > 10 then
            walk_to(entity.obj, entity.path, entity.progression, 3, 10 * time.delta * time.scale, true)
        end

        --entity.obj.components.transform:look_at(player_position,false,Vec3:new(0,1,0))
    end,
}

function UPDATE()
    if global_data.pause ~=nil and global_data.pause < 1 then
        time:get()

        player_position = { x = 0, y = 0, z = 0 }

        if global_data.player_object_ptr ~= nil then
            local player = game_object(global_data.player_object_ptr)

            if player ~= nil then
                player_position = player.components.transform:get_global_position()
                local player_position_down = Vec3:new(player_position.x, player_position.y - 1000, player_position.z)
                local hit, hit_info = raycast_3D(player_position, player_position_down)

                if hit then
                    player_position = hit_info.position
                end
                --print("A",ret.position.x,ret.position.y,ret.position.z)
            end
        end



        for index, value in ipairs(entitys_list) do
            update_entity_map[value.type](value)
        end

        if time_to_clean_paths > 0.25 then
            clean_pre_calculated_paths()
            time_to_clean_paths = 0
        else
            time_to_clean_paths = time_to_clean_paths + time.delta
        end
    end
end

function COLLIDE(collision_info)
end

function END()
end

local actions_per_type = {
    test_entity = function(entity)
        local model_path = "3D Models/test_friend.gltf"

        local entity_physics_3D = entity.obj.components.physics_3D
        entity_physics_3D.boady_dynamic = boady_dynamics.kinematic
        entity_physics_3D.collision_shape = collision_shapes.convex
        entity_physics_3D.collision_mesh = mesh_location:new(model_path, "Cube.001")
        entity_physics_3D.scale = Vec3:new(1, 1, 1)
        entity_physics_3D.rotate_x = false
        entity_physics_3D.rotate_y = false
        entity_physics_3D.rotate_z = false
        entity_physics_3D.friction = 0
        entity_physics_3D.gravity_scale = 0
        entity_physics_3D.density = 1
        entity_physics_3D.triger = false
        entity_physics_3D:set()

        local entity_data = get_scene_3D(model_path)
        local entity_structures = cenary_builders.entity(entity.obj.object_ptr, 2, entity_data, "Shaders/mesh",
            true, false)

        entity.rig_obj = entity_structures.obj
        entity.rig_obj.components.transform.position.y = -1.5
        entity.rig_obj.components.transform:set()
        entity.parts_ptr_list = entity_structures.parts_ptr_list
    end,
}

function summon_entity(args)
    local pos = args.pos
    local rot_y = args.rot_y
    local type = args.type

    local obj = game_object(create_object(global_data.layers.main))
    obj.components.transform.position = deepcopy(pos)
    obj.components.transform.rotation = { x = 0, y = rot_y, z = 0 }
    obj.components.transform:set()

    local entity = {
        type = type,
        obj = obj,
        rig_obj = nil,
        parts_ptr_list = nil,
        animation = "",
        animation_time = 0,
    }



    actions_per_type[type](entity)



    table.insert(entitys_list, entity)

    return {}
end

function remove_entity(adres)
    remove_object(entitys_list[adres].obj.object_ptr)
    table.remove(entitys_list, adres)
end

function clear(args)
    for index, value in pairs(entitys_list) do
        remove_object(value.obj.object_ptr)
    end
    entitys_list = {}
    return {}
end
