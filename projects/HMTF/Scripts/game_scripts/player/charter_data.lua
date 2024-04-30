require("components.extras")
require("components.component_all")

require("objects.global_data")
require("objects.local_data")
require("objects.time")
require("components.audio_source")

require("components.component_index")
require("objects.game_object")


require("short_cuts.create_render_shader")

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

health_display = {}
power_display = {}

function START()
    this_object = game_object(this_object_ptr)

    speaker = game_object(create_object(this_object_ptr))

    au = speaker.components.audio_source 

    my_data = local_data(this_object_ptr)
    my_data.health = 100

    --health_display
    local mat = matreial:new("health_display")
    mat.textures[1] = "Textures/ui/hud_life_energy.svg"
    mat.color = {r=0.0,g=0.5,b=1,a=1}
    mat.inputs["power"] = 0.25
    health_display = create_render_shader(global_data.layers.hud,true,Vec3:new(-0.1,0,0),Vec3:new(0,0,0),Vec3:new(0.25,0.25,0.25),5,mat)

    mat.inputs["power"] = 0.25
    mat.color = {r=1.0,g=1.0,b=0.0,a=1}
    power_display = create_render_shader(global_data.layers.hud,true,Vec3:new(0.1,0,0),Vec3:new(0,0,0),Vec3:new(-0.25,0.25,0.25),5,mat)
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end
