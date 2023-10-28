register_function_set("script")

require("components.base_component")
require("components.component_index")

lua_scripts_component = create_base_component(components.lua_scripts)
lua_scripts_component.object_ptr = ""
lua_scripts_component.scripts = {}
function lua_scripts_component:add_script(script_name)
    c_add_script_lua(self.object_ptr,script_name)
end
function lua_scripts_component:remove_script(script_name)
    c_remove_script(self.object_ptr,script_name)
end
function lua_scripts_component:get_variable(script_name,variable_name)
    return c_get_lua_var(self.object_ptr,script_name,variable_name)
end
function lua_scripts_component:set_variable(script_name,variable_name,value)
    c_set_lua_var(self.object_ptr,script_name,variable_name,value)
end
function lua_scripts_component:call_function(script_name,function_name,args_table)
    return c_call_lua_function(self.object_ptr,script_name,function_name,args_table)
end
function lua_scripts_component:has_script(script)
    self.scripts = deepcopyjson(c_get_lua_component(self.object_ptr).scripts)
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