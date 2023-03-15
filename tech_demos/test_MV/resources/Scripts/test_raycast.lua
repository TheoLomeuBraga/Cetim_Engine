require("TMP_libs.definitions")
require("TMP_libs.objects.material")
require("TMP_libs.objects.game_object")
require("TMP_libs.time")
require("TMP_libs.vectors")

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
    cube = create_mesh(nil,false,Vec3:new(0,0,0),Vec3:new(0,0,0),Vec3:new(1,1,1),2,{mat},{mesh_location:new("resources/3D Models/cube.obj","cube")})
end

function UPDATE()
    Time:get()
    ray_info.angle = ray_info.angle + Time.delta
    rc_ret = raycast_2D(ray_info.position,ray_info.max_distance,ray_info.angle)
    if rc_ret.position ~= nil then
        print("raycast colision point: ",rc_ret.position.x,rc_ret.position.y,rc_ret.position.z)
    end
    
end

function COLLIDE(collision_info)
end

function END()
    print("raycast test end")
    remove_object(cube.object_ptr)
end