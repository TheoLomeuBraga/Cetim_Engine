require("engine_libs.components.base_component")
require("engine_libs.components.component_index")





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

lua_scripts_component = create_base_component(components.lua_scripts)
lua_scripts_component.object_ptr = ""
lua_scripts_component.scripts = {}
function lua_scripts_component:add_script(script_name)
    add_script_lua(self.object_ptr,script_name)
end
function lua_scripts_component:remove_script(script_name)
    remove_script(self.object_ptr,script_name)
end
function lua_scripts_component:get_variable(script_name,variable_name)
    return get_lua_var(self.object_ptr,script_name,variable_name)
end
function lua_scripts_component:set_variable(script_name,variable_name,value)
    set_lua_var(self.object_ptr,script_name,variable_name,value)
end
function lua_scripts_component:call_function(script_name,function_name,args_table)
    return call_lua_function(self.object_ptr,script_name,function_name,args_table)
end
function lua_scripts_component:has_script(script)
    self.scripts = deepcopyjson(get_lua_component(self.object_ptr).scripts)
    for key, value in pairs(self.scripts) do
        if script == value then
            return true
        end
    end
    return false
end
function lua_scripts_component:get()
    self.scripts = deepcopyjson(get_lua_component(self.object_ptr).scripts)
end
function lua_scripts_component:set()
end

component_map[components.lua_scripts] = lua_scripts_component:new(nil)