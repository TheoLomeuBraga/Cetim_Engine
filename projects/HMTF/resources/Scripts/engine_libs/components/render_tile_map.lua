register_function_set("render")

require("engine_libs.components.base_component")
require("engine_libs.components.component_index")
require("engine_libs.objects.material")

render_tile_map_component = create_base_component(components.render_tile_map)
render_tile_map_component.layer = 2
render_tile_map_component.material = matreial:new()
render_tile_map_component.render_tilemap_only_layer = -1
render_tile_map_component.tile_set_local = ""
render_tile_map_component.tile_map_local = ""
function render_tile_map_component:clean()
    self.layer = 2
    self.material = matreial:new()
    self.render_tilemap_only_layer = -1
    self.tile_set_local = ""
    self.tile_map_local = ""
end
    function render_tile_map_component:get()
        j = get_set_render_tilemap(get_lua,self.object_ptr)
        self.layer = j.layer
        self.material = deepcopyjson(j.material)
        self.render_tilemap_only_layer = j.render_tilemap_only_layer
        self.tile_set_local = j.tile_set_local
        self.tile_map_local = j.tile_map_local
    end
    function render_tile_map_component:set()
        get_set_render_tilemap(set_lua,deepcopyjson(self))
    end
component_map[components.render_tile_map] = render_tile_map_component:new(nil)