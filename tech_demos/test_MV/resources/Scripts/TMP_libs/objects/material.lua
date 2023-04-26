require("TMP_libs.vectors")

matreial = {}
function matreial:new()
    return {
        shader = "",
        color = {r = 1,g = 1,b = 1,a = 1},
        position_scale = {x = 1,y = 1,z = 1,w = 1},
        metallic = 0 ,
        softness = 0,
        textures = {},
        inputs = {},
    }
end