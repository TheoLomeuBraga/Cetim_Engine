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


arms_data = nil
arms_objs = nil




function START()
    

    this_object = game_object(this_object_ptr)
    camera = game_object(this_object.components.lua_scripts:get_variable("game_scripts/player/charter_movement","camera_ptr"))
    
    arms_data = get_scene_3D("3D Models/charters/magic_arms.glb")
    arms_objs = cenary_builders.entity(camera.object_ptr, 4, arms_data, "arm_mesh", false, false)
    set_keyframe("3D Models/charters/magic_arms.glb", arms_objs.parts_ptr_list, true, "normal",0)
end




function shoot()
    
    
    
    --summon_bullet(bullet_types.ray,"","",pos,target,0,{r=1,g=0,b=0,a=1},100,1)
    --summon_bullet(bullet_types.fast,"","",pos,target,25,{r=1,g=0,b=0,a=1},100,1,"")

    
end


function UPDATE()

    
    
    if global_data.pause < 1 then
        time:get()

        
        
    end
end

function COLLIDE(collision_info)
end

function END()
end
