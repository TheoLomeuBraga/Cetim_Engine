function get_scene_3D(path)
end

function object_3D_example()
    return {
        name = "obj",
        id = 1,
        position = {x=0,y=0,z=0},
        rotation = {x=0,y=0,z=0},
        scale = {x=1,y=1,z=1},
        
        meshes = {},
        materials = {},

        variables = {},
        children = {},
    }
end

function key_frame_example_1()
    return {
        target_id = 1,

        has_position = true,
        has_scale = true,
        has_rotation = true,

        position = {x=0,y=0,z=0},
        rotation = {x=0,y=0,z=0},
        scale = {x=1,y=1,z=1},
    }
end

function key_frame_example_2()
    return {
        target_id = 1,

        has_position = true,
        has_scale = true,
        has_rotation = true,

        position = {x=0,y=1,z=0},
        rotation = {x=0,y=45,z=0},
        scale = {x=2,y=1,z=2},
    }
end

function example_animations()
    return {
        name = "",
        duration = 1,
        key_frames = {key_frame_example_1(),key_frame_example_2()},
    }
end

function sceane_3D_example()
    return {
        path = "",
        objects = {object_3D_example()},
        animations = {example_animations()},
        extra = {},
    }
end

