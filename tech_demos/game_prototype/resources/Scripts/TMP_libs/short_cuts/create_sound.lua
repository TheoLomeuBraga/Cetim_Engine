require("TMP_libs.components.extras")
require("TMP_libs.objects.game_object")
require("TMP_libs.components.transform")
require("TMP_libs.components.audio_source")

function create_audio(father,music_path,loop,volume)
    ret = game_object:new(create_object(father)) 
    ret:add_component(components.audio_source)
    ret.components[components.audio_source].path = music_path
    ret.components[components.audio_source].loop = loop
    ret.components[components.audio_source].volume = volume
    ret.components[components.audio_source]:set()
    return ret
end


function create_3D_audio(father,pos,music_path,loop,volume,min_distance,atenuation)
    ret = game_object:new(create_object(father)) 
    ret:add_component(components.transform)
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform]:set()
    ret:add_component(components.audio_source)
    ret.components[components.audio_source].path = music_path
    ret.components[components.audio_source].loop = loop
    ret.components[components.audio_source].volume = volume
    ret.components[components.audio_source].min_distance = min_distance
    ret.components[components.audio_source].atenuation = atenuation
    ret.components[components.audio_source]:set()
    return ret
end

