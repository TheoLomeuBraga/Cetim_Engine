register_function_set("audio")

require("components.base_component")
require("components.component_index")

function get_set_global_volume(volume)
    if volume == nil then
        return c_get_set_global_volume()
    else
        c_get_set_global_volume(volume)
    end
end

function set_lisener_object(obj)
    c_set_lisener_object(obj)
end

function get_set_audio(get_set, obj)
    return c_get_set_audio(get_set, obj)
end

audio_component = create_base_component(components.audio_source)
audio_component.path = "Audio/teste de audio.wav"
audio_component.pause = false
audio_component.loop = false
audio_component.time = 0.001
audio_component.speed = 1
audio_component.volume = 5
audio_component.min_distance = 1
audio_component.atenuation = 10

function audio_component:clean()
    self.path = "Audio/teste de audio.wav"
    self.pause = false
    self.loop = false
    self.time = 0.001
    self.speed = 1
    self.volume = 5
    self.min_distance = 1
    self.atenuation = 10
end

function audio_component:get()
    j = get_set_audio(get_lua, self.object_ptr)
    self.layer = j.layer
    self.material = deepcopyjson(j.material)
    self.render_tilemap_only_layer = j.render_tilemap_only_layer
    self.tile_set_local = j.tile_set_local
    self.tile_map_local = j.tile_map_local
end

function audio_component:set()
    get_set_audio(set_lua, deepcopyjson(self))
end

component_map[components.audio_source] = apply_component_metatable(audio_component:new(nil))
