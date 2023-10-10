register_function_set("memory")
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

function clear_memory()
    c_clear_memory()
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

function new_asset_list_element(type,path)
    return {
        type = type,
        path = path,
    }
end

--[[
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
]]
function stop_coroutine_if_is_not_loaded(asset_list)

    for key, value in pairs(asset_list) do
        is_loaded(value.type,value.path,true)
    end
    
    for key, value in pairs(asset_list) do
        while not is_loaded(value.type,value.path,false) do
            print("yield",is_loaded(value.type,value.path,false))
            coroutine.yield()
        end
    end
    
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
