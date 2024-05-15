require("objects.game_object")
require("components.component_all")
require("objects.time")
require("function_sets.simple_ui")

speed = 50

obj_ptr_to_rotate = nil
local obj_to_rotate = nil

function START()
    if obj_ptr_to_rotate ~= nil then
        obj_to_rotate = game_object(obj_ptr_to_rotate)
    end
end

local sw = stopwatch:new()

local tf = simple_ui_transform()
local style = simple_ui_style({r=0,g=0,b=0,a=1}, 0, {r=0,g=0,b=0,a=1}, {r=1,g=1,b=0,a=1})

local tf2 = simple_ui_transform()
local style2 = simple_ui_style({r=1,g=1,b=0,a=1}, 0, {r=0,g=0,b=0,a=1}, {r=1,g=1,b=0,a=1})
style2.image = "Textures/spiral.svg"

local rot = 0

function UPDATE()
    time:get()

    
    
    rot = rot + (speed * sw:get())

    simple_ui_display(tf, "Loading", style)
    simple_ui_display(simple_ui_transform({x=0.8,y=-0.8,z=0},{x=0.1,y=0.1,z=0.1},true,{x=0,y=0,z=-rot}), "", style2)
end

function COLLIDE(collision_info)
end

function END()
end
