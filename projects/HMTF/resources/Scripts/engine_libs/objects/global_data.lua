function global_data_get_var(var_name)
end

function global_data_set_var(var_name,var_value)
end

global_data = {}
function global_data:get_var(var_name)
    return global_data_get_var(var_name)
end

function global_data:set_var(var_name,var_value)
    global_data_set_var(var_name,var_value)
end