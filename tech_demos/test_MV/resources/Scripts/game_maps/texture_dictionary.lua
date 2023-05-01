
local textures_dictionary = {
    place_holder = {
        shader = "resources/Shaders/mesh",
        color = {r = 1,g = 1,b = 1,a = 1},
        position_scale = {x = 1,y = 1,z = 1,w = 1},
        metallic = 0 ,
        softness = 0,
        textures = {"resources/Textures/place holder.png"},
        inputs = {},
    },

    white = {
        shader = "resources/Shaders/mesh",
        color = {r = 1,g = 1,b = 1,a = 1},
        position_scale = {x = 1,y = 1,z = 1,w = 1},
        metallic = 0 ,
        softness = 0,
        textures = {"resources/Textures/white.png"},
        inputs = {},
    },

    
}

return function (texture)
    ret = textures_dictionary[texture]
    if ret == nil then
        ret = textures_dictionary.place_holder
    end
    return ret
end