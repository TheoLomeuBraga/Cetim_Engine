material = {}
function material:new()
    m = {}
    m.shader = ""
    m.color = {}
    m.color.r = 1
    m.color.g = 1
    m.color.b = 1
    m.color.a = 1
    m.position_scale = Vec4:new(0,0,1,1)
    m.metallic = 0 
    m.softness = 0
    m.textures = {"",}
    m.inputs = {0,}
    return m
end