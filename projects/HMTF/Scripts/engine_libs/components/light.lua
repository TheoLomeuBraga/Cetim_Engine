register_function_set("render")

require("components.base_component")
require("components.component_index")

light_component = create_base_component(components.light)
function light_component:clean()
    self.type = ""
    
end

function light_component:get()
    local j = get_set_light(get_lua,self.object_ptr)
    
end
function light_component:set()
    get_set_light(set_lua,deepcopyjson(self))
end
component_map[components.light] = apply_component_metatable(light_component:new(nil))