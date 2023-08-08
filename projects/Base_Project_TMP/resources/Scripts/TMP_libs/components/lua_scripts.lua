require("TMP_libs.components.component_index")





function have_script(object,script)
end
function add_script_lua(object,script)
end
function remove_script(object,script)
end
function get_lua_component()
end
function get_lua_var(object,script_name,variable_name)
end
function set_lua_var(object,script_name,variable_name,value)
end
function call_lua_function(object,script_name,function_name,arg)
end

lua_scripts_component = {}
function lua_scripts_component:new(object_ptr)
    ls = {}
    ls.object_ptr = object_ptr
    ls.scripts = {}
    function ls:get()
        self.scripts = deepcopyjson(get_lua_component(self.object_ptr).scripts)
    end
    function ls:set()
    end
    function ls:add_script(script_name)
        add_script_lua(self.object_ptr,script_name)
    end
    function ls:remove_script(script_name)
        remove_script(self.object_ptr,script_name)
    end
    function ls:get_variable(script_name,variable_name)
        return get_lua_var(self.object_ptr,script_name,variable_name)
    end
    function ls:set_variable(script_name,variable_name,value)
        set_lua_var(self.object_ptr,script_name,variable_name,value)
    end
    function ls:call_function(script_name,function_name,args_table)
        return call_lua_function(self.object_ptr,script_name,function_name,args_table)
    end
    
    function ls:delet()
        self.scripts = nil
        self = nil
    end
    return ls
end
component_map[components.lua_scripts] = lua_scripts_component