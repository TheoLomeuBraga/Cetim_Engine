require("TMP_libs.vectors")

matreial = {}
function matreial:new()
    m = {}
    m.shader = ""
    m.color = {r = 1,g = 1,b = 1,a = 1}
    m.position_scale = Vec4:new(0,0,1,1)
    m.metallic = 0 
    m.softness = 0
    m.textures = {}
    m.inputs = {}
    return m
end