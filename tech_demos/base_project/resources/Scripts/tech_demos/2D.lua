require("TMP_libs.short_cuts.load_2D_map")

json = require("libs.json")

local demo = {}
demo.map_data = {}
demo.map_objects = {}

function demo:START(layers)

    --"resources/Levels/2D/tile_map.json"
    --load_2D_map(father,pos,rot,sca,tile_map_path,tile_set_path,tile_set_images_folder,mat)



end

function demo:UPDATE()

end

function demo:END()
    clear_memory()
end

return demo