register_function_set("global_data")

function global_data_get_var(var_name)
    return c_global_data_get_var(var_name)
end

function global_data_set_var(var_name,var_value)
    return c_global_data_set_var(var_name,var_value)
end

global_data = {}


local global_data_metatable = {
    __index = function(self, key)
        return c_global_data_get_var(key)
    end,
    __newindex = function(self, key,value)
        c_global_data_set_var(key,value)
    end
}
setmetatable(global_data, global_data_metatable)