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
    hit_scan = false,
}

local wepom_list = {
    test_wepon = {
        file = "resources/3D Models/arm_cannon.gltf",
        automatic = false,
        fire_rate = 0.1,
        projectile_speed = 50,
        projectile_count = 12,
        spred = 1,
        hit_scan = false,
    }
}

avaliable_wepons = {"test_wepon",}

local current_animation_state = {
    name = "",
    loop = false,
    speed = 1,
    time = 0,
    finish = true,
}

function start_animation()
    
end

function run_animation()
    if current_animation_state ~= nil and current_animation_state.name ~= "" then
        time:get()
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

function select_wepon(wepon)
    
    local wepon_data = get_scene_3D(wepon.file)
    local objects = cenary_builders.entity(camera.object_ptr, 4, wepon_data,"resources/Shaders/explosive_vertex_mesh", false, false)

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
    
        automatic = wepon.automatic,
        fire_rate = wepon.fire_rate,
        projectile_speed = wepon.projectile_speed,
        projectile_count = wepon.projectile_count,
        spred = wepon.spred,
        hit_scan = wepon.hit_scan,
    }
    
end

function START()
    this_object = game_object:new(this_object_ptr)
    camera = game_object:new(this_object.components[components.lua_scripts]:get_variable("game_scripts/charter","camera_ptr"))
    select_wepon(wepom_list.test_wepon)
end

function shoot()
end

local hit_top = false
local hit_down = false
local movement_inpulse = {}

function get_charter_data()
    hit_top = this_object.components[components.lua_scripts]:get_variable("game_scripts/charter","hit_top")
    hit_down = this_object.components[components.lua_scripts]:get_variable("game_scripts/charter","hit_down")
    movement_inpulse = this_object.components[components.lua_scripts]:get_variable("game_scripts/charter","movement_inpulse")

end

function UPDATE()

    get_charter_data()

    

    run_animation()

end

function COLLIDE(collision_info)
end

function END()
end
