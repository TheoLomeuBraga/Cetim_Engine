require("definitions")
require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.game_object")
require("objects.time")
require("objects.material")
require("objects.global_data")
require("objects.vectors")
require("objects.input")
require("objects.gravity")
require("objects.scene_3D")
require("objects.window")
require("game_scripts.resources.playable_scene")


game_states = {
    play = 0,
    conversation = 1,
}
game_state = 0

this_object = nil
camera = nil

function START()
    global_data.interacting = 0
    this_object = game_object(this_object_ptr)
    camera = game_object(this_object.components.lua_scripts:get_variable("game_scripts/player/charter_movement","camera_ptr"))
end

interacting = false

function interact()
    interacting = true
    local ray_start = camera.components.transform:get_global_position(0, 0, 0)
    local ray_end_direction = camera.components.transform:get_local_direction(0, 0, -10)
    local ray_end = {
        x = ray_start.x - ray_end_direction.x,
        y = ray_start.y - ray_end_direction.y,
        z = ray_start.z - ray_end_direction.z
    }

    local hit = false
    local hit_info = {}
    hit, hit_info = raycast_3D(ray_start, ray_end)

    if hit then
        local hit_object = game_object(hit_info.collision_object)

        if hit_object.components ~= nil and hit_object.components.lua_scripts ~= nil and hit_object.components.lua_scripts:has_script("game_scripts/mensage") and inputs.interact > 0 and inputs_last_frame.interact < 1 then
            hit_object.components.lua_scripts:call_function("game_scripts/mensage", "interact", {})
        end
    end
end

function UPDATE()

    inputs = global_data.inputs
    inputs_last_frame = global_data.inputs_last_frame
    
    if global_data.interacting == 0 and inputs.interact > 0 and inputs_last_frame.interact < 1 then
        interact()
    end
    

end

function COLLIDE(collision_info)
end

function END()
end
