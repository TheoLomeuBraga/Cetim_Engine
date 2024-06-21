require("definitions")
require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.game_object")
require("objects.time")
require("objects.material")
require("objects.global_data")
require("objects.local_data")
require("objects.vectors")
require("objects.input")
require("objects.gravity")
require("objects.scene_3D")
require("game_scripts.resources.playable_scene")
require("short_cuts.create_collision")
require("game_scripts.resources.bullet_api")

require("math")

this_object = nil
camera = nil
base_arms = nil

arms_data = nil
arms_objs = nil


function change_spel_style(r,g,b,r2,g2,b2,speed)
    for index, value in ipairs(arms_objs.parts_ptr_list) do
        local obj = game_object(value)
        if obj.components.render_mesh then
            obj.components.render_mesh:get()

            for index, value in pairs(obj.components.render_mesh.materials) do
                value.color = {r=r,g=g,b=b,a=1}
                value.inputs.color_2_r = r2;
                value.inputs.color_2_g = g2;
                value.inputs.color_2_b = b2;
                value.inputs.perlin_speed = speed
            end

            obj.components.render_mesh:set()
        end
    end
end

function START()
    

    this_object = game_object(this_object_ptr)
    camera = game_object(this_object.components.lua_scripts:get_variable("game_scripts/player/charter_movement","camera_ptr"))
    base_arms = game_object(create_object(camera.object_ptr))
    base_arms.components.transform:set()

    arms_data = get_scene_3D("3D Models/charters/magic_arms.glb")
    arms_objs = cenary_builders.entity(base_arms.object_ptr, 4, arms_data, "arm_mesh", false, false)
    set_keyframe("3D Models/charters/magic_arms.glb", arms_objs.parts_ptr_list, true, "normal",0)

    change_spel_style(1,0,0,1,0.5,0,5)
end




function shoot()
    
    
    
    --summon_bullet(bullet_types.ray,"","",pos,target,0,{r=1,g=0,b=0,a=1},100,1)
    --summon_bullet(bullet_types.fast,"","",pos,target,25,{r=1,g=0,b=0,a=1},100,1,"")

    
end



animation_progresion = 0
function UPDATE()

    
    
    if global_data.pause < 1 then
        time:get()

        local inputs = global_data.inputs
        local rot_cam_x = math.min((inputs.mouse_view_x * 100) + inputs.analog_view_x,10)
        rot_cam_x = math.max(rot_cam_x,-10)

        local rot_cam_y = math.min((inputs.mouse_view_y * 100) + inputs.analog_view_y,10)
        rot_cam_y = math.max(rot_cam_y,-10)

        base_arms.components.transform:get()
        base_arms.components.transform.rotation.y = -rot_cam_x
        base_arms.components.transform.rotation.x = rot_cam_y
        base_arms.components.transform:set()

        animation_progresion = animation_progresion + time.delta

        if animation_progresion > arms_data.animations["normal"].duration then
            animation_progresion = 0
        end
        set_keyframe("3D Models/charters/magic_arms.glb", arms_objs.parts_ptr_list, true, "normal",animation_progresion)
        
    end
end

function COLLIDE(collision_info)
end

function END()
end
