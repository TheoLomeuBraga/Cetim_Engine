require("TMP_libs.short_cuts.load_2D_map")

json = require("libs.json")

local demo = {}
demo.map_data = {}
demo.map_objects = {}

function demo:START(layers)

    --"resources/Levels/2D/tile_map.json"
    local mat = matreial:new()
    mat.shader = "resources/Shaders/sprite"
    mat.color = {r=0,g=0,b=1,a=1}
    local data = load_2D_map(layers.cenary,{x=0,y=0,z=0},{x=0,y=0,z=0},{x=1,y=1,z=1},"resources/Levels/2D/tile_map.json","resources/Levels/2D/tile_set.json",mat)
    demo.map_data = deepcopy(data.tile_set_info)
    demo.map_objects = deepcopy(data.map_object)


end

function demo:UPDATE()

end

function demo:END()
    clear_memory()
end

return demo