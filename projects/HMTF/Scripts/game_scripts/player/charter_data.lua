require("components.extras")
require("components.component_all")

require("objects.global_data")
require("objects.time")
require("components.audio_source")

require("components.component_index")
require("objects.game_object")
require("objects.local_data")

this_object = {}

upgrades = {}

keys = {}

consumables = {}

au = {}


function play_pick_up_sound(args)
    
    
    
    --this_object.components.audio_source
    au:get()
    au.volume = 20

    if args[1] == "key" then

        au.path = "Audio/sounds/get_item.wav"
        
    elseif args[1] == "upgrade" then

        au.path = "Audio/sounds/get_item.wav"

    elseif args[1] == "consumable" then

        au.path = "Audio/sounds/get_item.wav"

    end
    au:set()
    return {}
end

my_data = {}

function START()
    this_object = game_object(this_object_ptr)

    speaker = game_object(create_object(this_object_ptr))

    au = speaker.components.audio_source 

    my_data = local_data(this_object_ptr)
    my_data.health = 100
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end
