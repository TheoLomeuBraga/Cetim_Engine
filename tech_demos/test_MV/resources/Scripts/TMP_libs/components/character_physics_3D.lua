require("TMP_libs.components.component_index")

function in_ceiling_3D(object)
end
function in_floor_3D(object)
end

character_physics_3D_component = {}
function character_physics_3D_component:new(object_ptr)
    p = {}
    p.object_ptr = object_ptr
    p.floor = false
    p.ceiling = false
    function p:get()
        --self.floor = in_floor_3D(self.object_ptr)
        --self.ceiling = in_ceiling_3D(self.object_ptr)
    end
    function p:set()
    end
    function p:delet()
        self = nil
    end
    return p
end
component_map[components.character_physics_3D] = character_physics_3D_component