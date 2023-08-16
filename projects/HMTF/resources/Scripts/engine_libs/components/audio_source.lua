require("engine_libs.components.base_component")
require("engine_libs.components.component_index")



--audio
function set_audio(object,path,pause,loop,time,speed,volume,min_distance,atenuation)
end
function get_audio(object)
end

function get_set_audio(get_set,object)
end
function set_lisener_object(object)
end
function get_set_global_volume(vol)
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