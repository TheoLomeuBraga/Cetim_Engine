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

function get_set_audio(get_set,obj)
    return c_get_set_audio(get_set,obj)
end

audio_component = {}
function audio_component:new(object_ptr)
    local a = {}
    a.object_ptr = object_ptr
    a.path = "resources/Audio/teste de audio.wav"
	a.pause = false
	a.loop = false
	a.time = 0.001
	a.speed = 1
    a.volume = 5
    a.min_distance = 1
    a.atenuation = 10

    function a:get()
        a = get_set_audio(get_lua,self.object_ptr)
        self.path = a.path
	    self.pause = a.pause > 0
	    self.loop = a.loop > 0
	    self.time = a.time
	    self.speed = a.speed
        self.volume = a.volume
        self.min_distance = a.min_distance
        self.atenuation = a.atenuation
    end
    function a:set()
        get_set_audio(set_lua,deepcopyjson(self))
    end

    return a
end
component_map[components.audio_source] = audio_component