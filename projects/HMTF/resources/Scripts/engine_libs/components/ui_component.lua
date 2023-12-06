register_function_set("render")

require("components.base_component")
require("components.component_index")


ui_component = create_base_component(components.ui_component)


function ui_component:clean()
end

function ui_component:get()
end
function ui_component:set()
end
component_map[components.ui_component] = apply_component_metatable(camera_component:new(nil))