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
function get_scene_3D_json(path)
end

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
