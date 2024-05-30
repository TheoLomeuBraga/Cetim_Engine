require("components.extras")
require("components.component_all")

require("objects.global_data")
require("objects.local_data")
require("objects.time")
require("components.audio_source")
require("function_sets.simple_ui")
require("components.component_index")
require("objects.game_object")
require("game_scripts.resources.playable_scene")
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

local health_transform = {}
local health_style = {}

local power_transform = {}
local power_style = {}


avatar_3D_data = {}
base_avatar = {}
avatar_3D = {}
avatar_created = false

function create_avatar()
    if global_data.camera_ptr ~= nil then

        
        local model_path = "3D Models/charters/female_charter.glb"

        avatar_3D_data = get_scene_3D(model_path)

        base_avatar = game_object(create_object(global_data.camera_ptr))

        base_avatar.components.transform:get()
        base_avatar.components.transform.position = { x = 2, y = -8, z = 10 }
        base_avatar.components.transform.rotation = { x = 0, y = 180 + 45, z = 0 }
        base_avatar.components.transform.scale = { x = 0.8, y = 0.8, z = 0.8 }

        base_avatar.components.transform.position = { x = 0, y = -20, z = 10 }
        base_avatar.components.transform.rotation = { x = 0, y = 180, z = 0 }
        base_avatar.components.transform.scale = { x = 3, y = 3, z = 3 }
        base_avatar.components.transform:set()


        avatar_3D = cenary_builders.entity(base_avatar.object_ptr, 4, avatar_3D_data, "mesh", true, false)

        set_keyframe(model_path, avatar_3D.parts_ptr_list, true, "normal", 1)
        set_keyframe(model_path, avatar_3D.parts_ptr_list, true, "expecion_test_mouth", 0)
        set_keyframe(model_path, avatar_3D.parts_ptr_list, true, "expecion_test_eyes", 0)
        
        avatar_created = true
    end
end

function START()
    this_object = game_object(this_object_ptr)

    speaker = game_object(create_object(this_object_ptr))

    au = speaker.components.audio_source

    my_data = local_data(this_object_ptr)
    my_data.health = 100.0
    my_data.max_health = 100.0

    my_data.power = 100.0
    my_data.max_power = 100.0


    health_transform = simple_ui_transform({ x = -0.05, y = 0, z = 0 }, { x = 0.1, y = 0.1, z = 0.1 })
    health_style = simple_ui_style({ r = 0.25, g = 0.25, b = 1, a = 1 })
    health_style.shader = "ui_element_heath_bar"
    health_style.image = "Textures/ui/hud_life_energy.svg"

    power_transform = simple_ui_transform({ x = 0.05, y = 0, z = 0 }, { x = -0.1, y = 0.1, z = 0.1 })
    power_style = simple_ui_style({ r = 1, g = 1, b = 0.25, a = 1 })
    power_style.shader = "ui_element_heath_bar"
    power_style.image = "Textures/ui/hud_life_energy.svg"
end

function UPDATE()
    if not avatar_created then
        create_avatar()
    end

    health_style.inputs.power = my_data.health / my_data.max_health
    power_style.inputs.power = my_data.power / my_data.max_power

    simple_ui_lable(health_transform, health_style)
    simple_ui_lable(power_transform, power_style)
end

function COLLIDE(collision_info)
end

function END()
end
