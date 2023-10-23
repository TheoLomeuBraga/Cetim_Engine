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
require("resources.playable_scene")
require("short_cuts.create_collision")
require("resources.bullet_api")

require("math")

this_object = nil
camera = nil


local selected_wepom = {
    file = "",
    data = {},
    obj = {},
    part_list = {},
    part_ptr_list = {},
    animations = {},

    automatic = false,
    fire_rate = 0.1,
    projectile_speed = 50,
    projectile_count = 12,
    spred = 1,
    speed = 1,
    hit_scan = false,
    color = { r = 1, g = 0, b = 0, a = 1 },

    damage = 1,
    life_time = 0.5,
    mesh = { file = "resources/3D Models/bullets.gltf", name = "round_bullet" },
}

local wepom_list = {
    test_wepon = {
        file = "resources/3D Models/arm_cannon.gltf",
        automatic = false,
        fire_rate = 0.1,
        projectile_speed = 50,
        projectile_count = 12,
        spred = 0.1,
        speed = 40,
        hit_scan = false,
        color = { r = 1, g = 0, b = 0, a = 1 },
        bullet_origens = { { x = -0.3, y = -0.3, z = 0 } },
        damage = 1,
        life_time = 0.5,
        mesh = { file = "resources/3D Models/bullets.gltf", name = "round_bullet" },
    }
}
next_shoot_timer = 0

avaliable_wepons = { "test_wepon", }

local current_animation_state = nil

function start_animation()

end

function run_animation()
    if current_animation_state ~= nil and current_animation_state.name ~= "" then
        current_animation_state.time = current_animation_state.time + time.delta
        if current_animation_state.time > current_animation_state.duration then
            if current_animation_state.loop then
                current_animation_state.time = 0
            else
                current_animation_state.time = current_animation_state.duration
                current_animation_state.finish = true
            end
        end


        set_keyframe(selected_wepom.file, selected_wepom.part_ptr_list, false, current_animation_state.name,current_animation_state.time)
    end
end

local bullet_start_pos_id = 1
function select_wepon(wepon)
    bullet_start_pos_id = 1
    local wepon_data = get_scene_3D(wepon.file)
    local objects = cenary_builders.entity(camera.object_ptr, 4, wepon_data, "resources/Shaders/explosive_vertex_mesh",false, false)
    --local objects = cenary_builders.scene(camera.object_ptr, 4, wepon_data, false)

    current_animation_state = {
        name = "pick_up",
        loop = false,
        speed = 1,
        time = 0,
        finish = false,
        duration = wepon_data.animations["pick_up"].duration,
    }

    selected_wepom = {
        file = wepon.file,
        data = wepon_data,
        obj = objects.obj,
        part_list = objects.parts_list,
        part_ptr_list = objects.parts_ptr_list,
        animations = wepon_data.animations,

        shoot_sound = "resources/Audio/sounds/shot_3.wav",

        automatic = wepon.automatic,
        fire_rate = wepon.fire_rate,
        projectile_speed = wepon.projectile_speed,
        projectile_count = wepon.projectile_count,
        spred = wepon.spred,
        speed = wepon.speed,
        hit_scan = wepon.hit_scan,
        color = wepon.color,
        bullet_origens = { { x = -0.3, y = -0.3, z = 0.5 }, },

        damage = wepon.damage,
        life_time = wepon.life_time,
        mesh = deepcopy(wepon.mesh),
    }
end

function START()
    this_object = game_object:new(this_object_ptr)
    camera = game_object:new(this_object.components[components.lua_scripts]:get_variable("game_scripts/charter","camera_ptr"))
    this_object:add_component(components.audio_source)

    select_wepon(wepom_list.test_wepon)
end

local movement_inpulse = { x = 0, y = 0, z = 0 }


function shoot()
    next_shoot_timer = selected_wepom.fire_rate

    bullet_start_pos_id = bullet_start_pos_id + 1
    if bullet_start_pos_id > #selected_wepom.bullet_origens then
        bullet_start_pos_id = 1
    end

    current_animation_state = {
        name = "shoot",
        loop = false,
        speed = 1,
        time = 0,
        finish = false,
        duration = selected_wepom.data.animations["shoot"].duration,
    }

    camera.components[components.audio_source].path = selected_wepom.shoot_sound
    camera.components[components.audio_source].volume = 20
    camera.components[components.audio_source]:set()
    --selected_wepom.bullet_origens

    local bullet_start_points = {}
    for i = 1, selected_wepom.projectile_count, 1 do
        local a = i
        if i > #selected_wepom.bullet_origens then
            a = (i % #selected_wepom.bullet_origens) + 1
        end
        bullet_start_points[a] = camera.components[components.transform]:get_global_position(
        selected_wepom.bullet_origens[a].x, selected_wepom.bullet_origens[a].y, selected_wepom.bullet_origens[a].z)
    end

    local ray_start = camera.components[components.transform]:get_global_position(0, 0, 0)
    local ray_end = camera.components[components.transform]:get_global_position(0, 0, 1000)
    local hit = false
    local hit_info = {}
    hit, hit_info = raycast_3D(ray_start, ray_end)
    local target = deepcopy(ray_end)

    local normalize = function(vec3)
        local sun = math.abs(vec3.x) + math.abs(vec3.y) + math.abs(vec3.z)
        return { x = vec3.x / sun, y = vec3.y / sun, z = vec3.z / sun }
    end

    for i = 1, selected_wepom.projectile_count, 1 do
        local spred_direction = camera.components[components.transform]:get_local_direction(0, 0, 1)

        if selected_wepom.spred > 0 then
            spred_direction = camera.components[components.transform]:get_local_direction(
            (math.random() - 0.5) * selected_wepom.spred, (math.random() - 0.5) * selected_wepom.spred, 0)
        end

        --local a = (i % #selected_wepom.bullet_origens) + 1
        summon_bullet(bullet_start_points[bullet_start_pos_id], normalize(ray_end), selected_wepom.mesh,
            { spred_direction }, selected_wepom.speed, selected_wepom.life_time, selected_wepom.damage, 1,
            selected_wepom.hit_scan, movement_inpulse, true, true, selected_wepom.color, "")
    end
end

local hit_top = false
local hit_down = false

local inpulse = {}

inputs = {}
inputs_last_frame = {}

function get_charter_data()
    hit_top = this_object.components[components.lua_scripts]:get_variable("game_scripts/charter", "hit_top")
    hit_down = this_object.components[components.lua_scripts]:get_variable("game_scripts/charter", "hit_down")
    movement_inpulse = this_object.components[components.lua_scripts]:get_variable("game_scripts/charter","movement_inpulse")



    inputs = global_data:get("inputs")
    inputs_last_frame = global_data:get("inputs_last_frame")
end

function UPDATE()
    if global_data:get("pause") < 1 then
        time:get()

        get_charter_data()

        if current_animation_state.name ~= "shoot" or current_animation_state.name == "shoot" and current_animation_state.finish then
            if hit_down and math.abs(movement_inpulse.x) + math.abs(movement_inpulse.z) > 0 and current_animation_state.name ~= "walk" and  ( current_animation_state.name ~= "pick_up" or current_animation_state.name == "pick_up" and current_animation_state.finish) then
                current_animation_state = {
                    name = "walk",
                    loop = true,
                    speed = 1,
                    time = 0,
                    finish = false,
                    duration = selected_wepom.data.animations["walk"].duration,
                }
            elseif math.abs(movement_inpulse.x) + math.abs(movement_inpulse.z) == 0 and current_animation_state.name == "walk" and ( current_animation_state.name ~= "pick_up" or current_animation_state.name == "pick_up" and current_animation_state.finish) or not hit_down then
                current_animation_state = {
                    name = "normal",
                    loop = true,
                    speed = 1,
                    time = 0,
                    finish = false,
                    duration = selected_wepom.data.animations["normal"].duration,
                }
            end
        end
        

        

        if next_shoot_timer > 0 then
            next_shoot_timer = next_shoot_timer - time.delta
        end
        if next_shoot_timer < 0 then
            next_shoot_timer = 0
        end

        if next_shoot_timer == 0 and inputs.action_1 > 0 then
            if selected_wepom.automatic then
                shoot()
            elseif inputs_last_frame.action_1 < 1 then
                shoot()
            end
        end

        

        if current_animation_state ~= nil then
            run_animation()
        end
        
    end
end

function COLLIDE(collision_info)
end

function END()
end
