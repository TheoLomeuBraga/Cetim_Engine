--[[


vec3 pos;
float velocidade;
vec3 nor;
]]
collision_info = {}
function collision_info:new()
    ret = {
        object = "",
        collision_object = "",
        trigrer = false,
        colliding = false,
        distance = 0,
        position = {x=0,y=0,z=0},
        speed = 0,
        normal = {x=0,y=0,z=0}
    }
    return ret
end