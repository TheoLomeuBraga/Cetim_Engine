register_function_set("global_data")

function global_data_get_var(var_name)
    return c_global_data_get_var(var_name)
end

function global_data_set_var(var_name,var_value)
    return c_global_data_set_var(var_name,var_value)
end

global_data = {}
function global_data:get_var(var_name)
    return global_data_get_var(var_name)
end

function global_data:set_var(var_name,var_value)
    global_data_set_var(var_name,var_value)
end
function global_data:get(var_name)
    return global_data_get_var(var_name)
end

function global_data:set(var_name,var_value)
    global_data_set_var(var_name,var_value)
end

local global_data_metatable = {
    __index = function(self, key)
        return global_data_get_var(key)
    end,
    __newindex = function(self, key,value)
        global_data_set_var(key,value)
    end
}
setmetatable(global_data, global_data_metatable)