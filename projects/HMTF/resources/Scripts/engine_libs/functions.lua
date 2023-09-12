require("math")

function splitString(inputString, delimiter)
    local result = {}
    local pattern = string.format("([^%s]+)", delimiter)
    
    for match in inputString:gmatch(pattern) do
        table.insert(result, match)
    end
    
    return result
end

function deepcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopy(orig_key)] = deepcopy(orig_value)
        end
        setmetatable(copy, deepcopy(getmetatable(orig)))
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
end

function deepprint(tab)
    local orig_type = type(tab)
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, tab, nil do
            print(orig_key, orig_value)
            if type(orig_value) == 'table' then
                print("{")

                deepprint(orig_value)

                print("}")
            end
        end
    else -- number, string, boolean, etc
        print(tab)
    end
end

function tableprint(tab)
    local orig_type = type(tab)
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, tab, nil do
            print(orig_key, orig_value)
        end
    else -- number, string, boolean, etc
        print(tab)
    end
end

function deepcopyjson(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopyjson(orig_key)] = deepcopyjson(orig_value)
        end
        setmetatable(copy, deepcopyjson(getmetatable(orig)))
    else
        if orig_type ~= 'function' then
            copy = orig
        end
    end
    return copy
end

function tablelength(T)
    local count = 0
    for _ in pairs(T) do
        count = count + 1
    end
    return count
end

function lerp(A,B,time)
    return (A * time) + (B * (1 - time))
end

asset_types = {
    image = 1,
    font = 2,
    tile_set = 3,
    tile_map = 4,
    scene_3D = 5,
}
-- Function that check if the asset are loaded
-- @param asset_type number: The asset type
-- @param asset_path string: The path to the asset
-- @param load boolean: Showd i load it if it isant loaded
-- @return boolean: The asset is loaded ?
function is_loaded(asset_type,asset_path,load)
end

function new_asset_list_element(type,path)
    return {
        type = type,
        path = path,
    }
end
function stop_coroutine_if_is_not_loaded(asset_list)
    for key, value in pairs(asset_list) do
        wait = true
        while wait do
            wait = false
            while not is_loaded(value.type,value.path,true) do
                wait = true
            end
            if wait then
                coroutine.yield()
            end
        end
    end
end

-- Function that calculates the sum of two numbers
-- @param a number: The first operand
-- @param b number: The second operand
-- @return number: The sum of a and b
function get_scene_3D(path)
end

-- time
function get_time()
end
function set_time_scale(speed)
end

-- memory
function clear_memory()
end

function loading_requests_number()
end

function get_set_parallel_loading(get_set,value)
end

function asset_is_load(asset_type, path)
end

-- Function to convert a normalized vector to angles in degrees
function vectorToAngles(normal)
    local pitch = math.deg(math.asin(normal.y))
    local yaw = math.deg(math.atan2(normal.x, normal.z))

    return {x = pitch,y =  yaw,z = 0} -- Roll is set to 0 since it's not well-defined for a normal vector
end

-- Function to calculate the cross product of two vectors
function crossProduct(a, b)
    local result = {
        x = a.y * b.z - a.z * b.y,
        y = a.z * b.x - a.x * b.z,
        z = a.x * b.y - a.y * b.x
    }
    return result
end

-- Function to calculate the dot product of two vectors
function dotProduct(a, b)
    return a.x * b.x + a.y * b.y + a.z * b.z
end

-- Function to add two vectors
function sumVectors(a, b)
    return {x = a.x + b.x, y = a.y + b.y, z = a.z + b.z}
end

function multplayVectors(a, b)
    return {x = a.x * b.x, y = a.y * b.y, z = a.z * b.z}
end

-- Function to multiply a vector by a scalar
function multiplyVectorByScalar(vector, scalar)
    return {x = vector.x * scalar, y = vector.y * scalar, z = vector.z * scalar}
end

function divideVectors(a, b)
    return {x = a.x / b.x, y = a.y / b.y, z = a.z / b.z}
end

-- Function to subtract one vector from another
function subtractVectors(a, b)
    return {x = a.x - b.x, y = a.y - b.y, z = a.z - b.z}
end



-- asset tiles
--[[
function get_tile_set_size(path,image_folder)
end
function get_tile_set_tile(path,image_folder,tile_id)
end

function get_tilemap_size(path)
end
function get_tilemap_layer_size(path)
end
function get_tilemap_data(path,layer,id)
end
]]

-- asset 3D scene
-- adicionar lua cena 3D

-- moovement
function to_move(object, x, y)
end
function to_move(object, x, y, z)
end



-- physic
function get_physic_2D(object)
end
function set_physic_2D(object, scale_x, scale_y, dynamic, shape, rotate, is_triger)
end

function get_physic_layer(object)
end
function set_physic_layer(object, layer)
end
function get_layers_can_colide_size(object)
end
function set_layers_can_colide_size(object, size)
end

function get_collisions_number(object)
end
function get_collision_object(object, id)
end

function set_layers_can_colide(object, id_layer, layer)
end
function get_layers_can_colide(object, id_layer)
end

-- render
function set_resolution(x, y)
end
function get_render_layer(object)
end
function set_render_layer(object, layer)
end

function get_set_window(get_set, object)
end

--[[
--material
function get_max_textures_material()
end
function get_max_inputs_material()
end

function set_shader(object,material_id,shader)
end
function get_shader(object,material_id)
end

function get_color(object,material_id)
end
function set_color(object,material_id,r,g,b,a)
end

function get_material_position_scale(object,material_id)
end
function set_material_position_scale(object,material_id,position_X,position_Y,scale_X,scale_Y)
end

function set_texture(object,material_id,texture_id,texture)
end
function get_texture(object,material_id,texture_id)
end

function set_material_input(object,material_id,input_id,input)
end
function get_material_input(object,material_id,input_id)
end
]]

-- scripts

-- function set_var_script(object,script,variable,type,value)
-- end
-- function get_script_var(object,script,variable,type)
-- end

-- post-procesing

-- render layers
function get_render_layer_instruction_json()
end
function set_render_layer_instruction_json(json)
end
