


local textures_dictionary = {

    place_holder = {
        shader = "resources/Shaders/mesh",
        normal_direction = normal_direction.both,
        color = {r = 1,g = 1,b = 1,a = 1},
        position_scale = {x = 0,y = 0,z = 10,w = 10},
        metallic = 0 ,
        softness = 0,
        textures = {"resources/Textures/place holder.png"},
        texture_filter = {0},
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