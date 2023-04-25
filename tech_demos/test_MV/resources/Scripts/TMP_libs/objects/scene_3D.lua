function get_scene_3D(path)
end

function object_3D_example()
    return {
        name = "obj",
        position = {x=0,y=0,z=0},
        rotation = {x=0,y=0,z=0},
        scale = {x=0,y=0,z=0},
        
        variables = {},
        children = {},
    }
end

function sceane_3D_example()
    return {
        path = "",
        objects = object_3D_example(),
        extra = {},
    }
end

