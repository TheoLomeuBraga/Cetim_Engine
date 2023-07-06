require("TMP_libs.functions")

function get_scene_3D(path)
end

function object_3D_example()
    return {
        name = "obj",
        id = 1,
        position = {
            x = 0,
            y = 0,
            z = 0
        },
        rotation = {
            x = 0,
            y = 0,
            z = 0
        },
        scale = {
            x = 1,
            y = 1,
            z = 1
        },

        meshes = {},
        materials = {},

        variables = {},
        children = {}
    }
end

function create_keyframe_node(target_id, position, rotation, scale)
    local ret = {
        target_id = target_id,
        has_position = false,
        has_scale = false,
        has_rotation = false,

        position = {
            x = 0,
            y = 0,
            z = 0
        },
        rotation = {
            x = 0,
            y = 0,
            z = 0
        },
        scale = {
            x = 1,
            y = 1,
            z = 1
        }
    }

    if position ~= nil then
        ret.has_position = true
        ret.position = deepcopy(position)
    end

    if rotation ~= nil then
        ret.has_rotation = true
        ret.rotation = deepcopy(rotation)
    end

    if scale ~= nil then
        ret.has_scale = true
        ret.scale = deepcopy(scale)
    end

    return ret
end

function key_frame_set_example_1()
    return {
        [1] = {
            target_id = 1,

            has_position = true,
            has_scale = true,
            has_rotation = true,

            position = {
                x = 0,
                y = 0,
                z = 0
            },
            rotation = {
                x = 0,
                y = 0,
                z = 0
            },
            scale = {
                x = 1,
                y = 1,
                z = 1
            }
        }
    }
end

function key_frame_set_example_2()
    return {
        [1] = {
            target_id = 1,

            has_position = true,
            has_scale = true,
            has_rotation = true,

            position = {
                x = 0,
                y = 1,
                z = 0
            },
            rotation = {
                x = 0,
                y = 45,
                z = 0
            },
            scale = {
                x = 2,
                y = 1,
                z = 2
            }
        }
    }
end

function example_animations()
    return {
        name = "",
        duration = 1,
        key_frames = {key_frame_example_1(), key_frame_example_2()}
    }
end

function sceane_3D_example()
    return {
        path = "",
        objects = {object_3D_example()},
        animations = {example_animations()},
        extra = {}
    }
end

function lerp_key_frame(key_frame_A, key_frame_B, time)
    local ret = {}
    
    return ret
end

function apply_key_frame(game_objects_list, key_frame)
    for k, kfn in pairs(key_frame) do

        if kfn.has_position == 1 then
            game_objects_list[kfn.target_id].components[components.transform].position = deepcopy(kfn.position)
        end

        if kfn.has_rotation == 1 then
            game_objects_list[kfn.target_id].components[components.transform].rotation = deepcopy(kfn.rotation)
        end

        if kfn.has_scale == 1 then
            game_objects_list[kfn.target_id].components[components.transform].scale = deepcopy(kfn.scale)
        end

        game_objects_list[kfn.target_id].components[components.transform]:set()

    end
end
