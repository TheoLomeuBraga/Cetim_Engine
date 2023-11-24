
require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")

upgrades = {}

keys = {}

consumables = {}

function play_pick_up_sound(args)
    print(args[1])
    if args[1] == "key" then

    elseif args[1] == "upgrade" then

    elseif args[1] == "consumable" then
        
    end
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
