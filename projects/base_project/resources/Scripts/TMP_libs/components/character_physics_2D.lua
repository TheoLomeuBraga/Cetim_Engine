require("TMP_libs.components.component_index")

function in_ceiling(object)
end
function in_floor(object)
end

character_physics_2D_component = {}
function character_physics_2D_component:new(object_ptr)
    p = {}
    p.object_ptr = object_ptr
    p.floor = false
    p.ceiling = false
    function p:get()
        self.floor = in_floor(self.object_ptr)
        self.ceiling = in_ceiling(self.object_ptr)
    end
    function p:set()
    end
    function p:delet()
        self = nil
    end
    return p
end
component_map[components.character_physics_2D] = character_physics_2D_component