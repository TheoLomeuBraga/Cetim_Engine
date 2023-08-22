require("engine_libs.components.base_component")
require("engine_libs.components.component_index")
require("engine_libs.objects.material")

--sprite
function set_tileset(object,tileset)
end
function get_tileset(object)
end

function get_sprite_render_json(object)
end
function set_sprite_render_json(object,json)
end
function get_set_sprite_render(get_set,object)
end

render_sprite_component = create_base_component(components.render_sprite)
render_sprite_component.layer = 2
render_sprite_component.selected_tile = 0
render_sprite_component.tile_set_local = ""
render_sprite_component.material = matreial:new()
function render_sprite_component:clean()
    self.layer = 2
    self.selected_tile = 0
    self.tile_set_local = ""
    self.material = matreial:new()
end
    function render_sprite_component:get()
        j = get_set_sprite_render(get_lua,self.object_ptr)
        self.layer = j.layer
        self.selected_tile = j.selected_tile
        self.tile_set_local = j.tile_set_local
        self.material = deepcopyjson(j.material)

    end
    function render_sprite_component:set()
        get_set_sprite_render(set_lua,self)
    end
component_map[components.render_sprite] = render_sprite_component:new(nil)