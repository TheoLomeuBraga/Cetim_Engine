require("components.extras")
require("components.component_all")

require("objects.global_data")
require("objects.time")
require("components.audio_source")

require("components.component_index")
require("objects.game_object")

this_object = {}

upgrades = {}

keys = {}

consumables = {}

function play_pick_up_sound(args)
    
    this_object = game_object(this_object_ptr)
    local au = audio_component(this_object_ptr)--this_object.components.audio_source
    au:get()
    au.volume = 20

    if args[1] == "key" then

        au.path = "resources/Audio/sounds/jump.wav"
        
    elseif args[1] == "upgrade" then

        au.path = "resources/Audio/sounds/jump.wav"

    elseif args[1] == "consumable" then

        au.path = "resources/Audio/sounds/jump.wav"

    end
    au:set()
    return {}
end

function START()
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end
