--[[
require("TMP_libs.definitions")
require("TMP_libs.objects.material")
require("TMP_libs.objects.game_object")
require("TMP_libs.time")
require("TMP_libs.vectors")
require("TMP_libs.short_cuts.create_mesh")
]]
require("TMP_libs.definitions")
require("TMP_libs.components.component_table")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.material")
require("TMP_libs.objects.game_object")
require("TMP_libs.time")
require("TMP_libs.vectors")
require("TMP_libs.short_cuts.create_mesh")


father = ""
cube = {}
ray_info = {
    position = {x=0,y=0},
    max_distance = 100,
    angle = 0,
}

function START()
    print("raycast test")
    
    mat = matreial:new()
    mat.shader = "resources/Shaders/mesh"
    mat.color = {r = 1,g = 0,b = 0,a = 1}
    cube = create_mesh(father,false,Vec3:new(0,0,0),Vec3:new(0,0,0),Vec3:new(1,1,1),2,{mat},{mesh_location:new("resources/3D Models/cube.obj","Cube")})
    
end

function UPDATE()
    --
    Time:get()
    ray_info.angle = ray_info.angle + Time.delta
    rc_hit,rc_ret = raycast_2D(ray_info.position,ray_info.max_distance,ray_info.angle)
    
    if rc_hit then
        cube.components[components.transform].position = Vec3:new(rc_ret.position.x,rc_ret.position.y,0)
    else
        cube.components[components.transform].position = Vec3:new(ray_info.position.x,ray_info.position.y,0)
    end
    cube.components[components.transform]:set()
    
    
end

function COLLIDE(collision_info)
end

function END()
    print("raycast test end")
    --[[
    remove_object(cube.object_ptr)
    ]]
end