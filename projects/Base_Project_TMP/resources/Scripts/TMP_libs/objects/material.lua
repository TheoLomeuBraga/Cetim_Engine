require("TMP_libs.objects.gravity")

normal_direction = {
    front = 0,
    back = 1,
	both = 2,
}

matreial = {}
function matreial:new()
    return {
        shader = "",
        normal_direction = normal_direction.front,
        color = {r = 1,g = 1,b = 1,a = 1},
        position_scale = {x = 0,y = 0,z = 1,w = 1},
        metallic = 0 ,
        softness = 0,
        textures = {},
        texture_filter = {},
        inputs = {},
    }
end