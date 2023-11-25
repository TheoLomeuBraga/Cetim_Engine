require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.global_data")
require("objects.time")
require("short_cuts.create_sound")
require("short_cuts.create_render_shader")

this_object = nil

rotate = true


item_type = "key"
item_name = "red"
amount = 1

function START()
    this_object = game_object:new(this_object_ptr)
end

local is_player_touch = function(objs_touching)
    for key, value in pairs(objs_touching) do
        local obj_touching = game_object:new(value)

        obj_touching.components[lua_scripts]:get()
        


        local charter_data = simple_lua_script_manager(obj_touching.object_ptr,"game_scripts/player/charter_data")

        if charter_data ~= false then

            
            
            if item_type == "key" then

                local keys = charter_data.variables.keys
                table.insert(keys,item_name)
                charter_data.variables.keys = keys

            elseif item_type == "upgrade" then 

                local upgrades = charter_data.variables.upgrades
                table.insert(upgrades,item_name)
                charter_data.variables.upgrades = upgrades

            elseif item_type == "consumable" then 
                
                local consumables = charter_data.variables.consumables
                if consumables[item_name] == nil then
                    consumables[item_name] = 0
                end
                consumables[item_name] = consumables[item_name] + amount
                charter_data.variables.consumables = consumables

            end

            --obj_touching.components[components.lua_scripts]:call_function("game_scripts/player/charter_data","play_pick_up_sound",{item_type})
            charter_data.functions.play_pick_up_sound({item_type})

            

            remove_object(this_object_ptr)
            return true
        end
    end
    return false
end

local rotation = 0

function UPDATE()
    local player_touch = is_player_touch(this_object.components[components.physics_3D]:get_objects_coliding())

    if rotate then
        time:get()

        this_object.components[components.transform]:change_rotation(0,rotation,0)
        rotation = rotation + (time.delta * time.scale * 180)
    end
end

function COLLIDE(collision_info)
end

function END()
end
