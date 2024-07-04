require("components.extras")
require("components.component_all")
require("components.component_index")
require("objects.input")
require("objects.time")
require("objects.global_data")
require("objects.window")
require("short_cuts.create_ui")
require("short_cuts.create_sound")
require("objects.window")
local serializer = require("libs.serialize")
require("math")
require("function_sets.simple_ui")



function START()
end

function update_ui()
    if global_data.ui_selection_id == nil then
        global_data.ui_selection_id = -1
    end
    if global_data.inputs ~= nil then
        local inputs = global_data.inputs
        local inputs_last_frame = global_data.inputs_last_frame

        local cursor_movement = {x=inputs.mouse_pos_x-cursor_pos_last_frame.x,y=cursor_pos_last_frame.y-inputs.mouse_pos_y}
        set_selection_state({x=inputs.mouse_pos_x,y=inputs.mouse_pos_y},cursor_movement,global_data.ui_selection_id,inputs.action_1)
        cursor_pos_last_frame = {x=inputs.mouse_pos_x,y=inputs.mouse_pos_y}
    end 

    
end

local selected_menu = 0

local style = simple_ui_style({r=0,g=0,b=0,a=0}, 0.0, {r=0,g=0,b=0,a=0}, {r=1,g=1,b=1,a=1})
local style_howver = simple_ui_style({r=0,g=0,b=0,a=0}, 0.0, {r=0,g=0,b=0,a=0}, {r=0.5,g=0.5,b=1,a=1})
local style_click = simple_ui_style({r=0,g=0,b=0,a=0}, 0.0, {r=0,g=0,b=0,a=0}, {r=0,g=0,b=1,a=1})

local start_tf = simple_ui_transform()



function UPDATE()

    if selected_menu == 0 then

        start_tf.scale = {x=0.5,y=0.5,z=0.5}

        if simple_ui_button(start_tf,"push me",style,1,style_howver) == 1 then
            
        end
        
        
        
    end

    update_ui()
    
end

function END()
end

function COLLIDE(collision_info)
end
