require("TMP_libs.components.component_index")
require("TMP_libs.objects.material")

--tilemap
function get_set_render_tilemap(get_set,object)
end

render_tile_map_component = {}
function render_tile_map_component:new(object_ptr)
    rtm = {}
    rtm.object_ptr = object_ptr
    rtm.layer = 1
    rtm.material = matreial:new()
    rtm.render_tilemap_only_layer = -1
    rtm.tile_set_local = ""
    rtm.tile_map_local = ""
    function rtm:get()
        j = get_set_render_tilemap(get_lua,self.object_ptr)
        self.layer = j.layer
        self.material = deepcopyjson(j.material)
        self.render_tilemap_only_layer = j.render_tilemap_only_layer
        self.tile_set_local = j.tile_set_local
        self.tile_map_local = j.tile_map_local
    end
    function rtm:set()
        get_set_render_tilemap(set_lua,deepcopyjson(self))
    end
    function rtm:delet()
    end
    return rtm
end
component_map[components.render_tile_map] = render_tile_map_component