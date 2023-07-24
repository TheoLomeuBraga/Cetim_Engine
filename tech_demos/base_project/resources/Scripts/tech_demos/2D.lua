require("TMP_libs.short_cuts.load_2D_map")

json = require("libs.json")

local demo = {}
demo.map_data = {}
demo.map_objects = {}

function demo:load_objects(layer_data,map_size_pixels)
    for key, value in pairs(layer_data.objects) do
        
    end
end

function demo:load_collision(layer_data,map_size_pixels)
    for key, value in pairs(layer_data.objects) do
        --create_collision_2D(father, pos, rot, sca, rigid_boady,shape,vertex_data,triger)
    end
end

function demo:START(layers)

    --"resources/Levels/2D/tile_map.json"
    local mat = matreial:new()
    mat.shader = "resources/Shaders/sprite"
    mat.color = {r=0,g=0,b=1,a=1}
    local data = load_2D_map(layers.cenary,{x=0,y=0,z=0},{x=0,y=0,z=0},{x=1,y=1,z=1},"resources/Levels/2D/tile_map.json","resources/Levels/2D/tile_set.json",mat)
    demo.map_data = deepcopy(data.tile_map_info)
    demo.map_objects = deepcopy(data.map_object)


    local map_size_pixels = {x = demo.map_data.width * demo.map_data.tilewidth,y = demo.map_data.height * demo.map_data.tileheight}
    for index, value in pairs(demo.map_data.layers) do
        if value.name == "objects" then
            demo:load_objects(value,map_size_pixels)
        elseif value.name == "collision" then
            demo:load_collision(value,map_size_pixels)
        end
    end

end

function demo:UPDATE()

end

function demo:END()
    remove_object(demo.map_objects.object_ptr)
    clear_memory()
end

return demo