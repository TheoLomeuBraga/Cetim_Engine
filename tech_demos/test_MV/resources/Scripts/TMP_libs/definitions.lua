argsn = 0
args = {}

function table.shallow_copy(t)
    local t2 = {}
    for k,v in pairs(t) do
      t2[k] = v
    end
    return t2
end

function shallow_copy(t)
    return table.shallow_copy(t)
end

--input
function get_input(input_divice,input_to_check)
end 
--mouse
mouse = {}
mouse.name = "mouse"

mouse.left = "left"
mouse.right = "right"
mouse.scroll_button = "scroll_button"

mouse.scroll = "scroll"
mouse.position_x = "position_x"
mouse.position_y = "position_y"

--joystick
joystick = {}
joystick.name = "joystick"

joystick.A ="A"
joystick.B ="B"
joystick.X ="X"
joystick.Y ="Y"

joystick.LB ="LB"
joystick.RB ="RB"
joystick.LT ="LT"
joystick.RT ="RT"

joystick.start = "start"
joystick.back = "back"

joystick.D_pad_up = "D_pad_up"
joystick.D_pad_down = "D_pad_down"
joystick.D_pad_left = "D_pad_left"
joystick.D_pad_right = "D_pad_right"

joystick.analog_left_X = "analog_left_X"
joystick.analog_left_Y = "analog_left_Y"
joystick.analog_right_X = "analog_right_X"
joystick.analog_right_Y = "analog_right_Y"

--keys
get_keys_input = false;
keys = {}
keys.text_input = ""





--camera types
camera_view_types = {}
camera_view_types.perspective = "perspective"
camera_view_types.ortho = "ortho"

--variable types
variable_types = {}
variable_types.string = 0
variable_types.number = 1
variable_types.bool = 2

--variable types
asset_types = {}
asset_types.font = "font"
asset_types.image = "image"
asset_types.tile_set = "tile_set"
asset_types.tile_map = "tile_map"
asset_types.audio = "audio"

--get set
get_lua = 0
set_lua = 1