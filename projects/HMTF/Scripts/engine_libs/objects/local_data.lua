register_function_set("script")


function local_data(obj_ptr)
    local ret = {__object_ptr__ = obj_ptr}
    local global_data_metatable = {
        __index = function(self, key)
            return c_local_data_get_var(self.__object_ptr__,key)
        end,
        __newindex = function(self, key,value)
            c_local_data_set_var(self.__object_ptr__,key,value)
        end
    }

    setmetatable(ret, global_data_metatable)
    return ret
end


