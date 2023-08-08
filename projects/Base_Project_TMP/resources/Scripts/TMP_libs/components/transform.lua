require("TMP_libs.components.component_index")
require("TMP_libs.objects.vectors")

--transform
function get_set_transform(get_set,object)
end

function move_transform(object,x,y,z)
end
function rotate_transform(object,x,y,z)
end

function change_transfotm_position(object,x,y,z)
end
function change_transfotm_rotation(object,x,y,z)
end
function change_transfotm_scale(object,x,y,z)
end
function get_local_direction_transform(object,x,y,z)
end
function get_translation_position_transform(object,x,y,z)
end
function get_global_position_transform(object,x,y,z)
end


transform_component = {}
function transform_component:new(object_ptr)
    t = {}
    t.object_ptr = object_ptr
    t.is_ui = false
    t.position = Vec3:new(0,0,0)
    t.rotation = Vec3:new(0,0,0)
    t.scale = Vec3:new(1,1,1)
    function t:get()
        t = get_set_transform(get_lua,self.object_ptr)
        self.is_ui = t.is_ui
        self.position = deepcopyjson(t.position)
        self.rotation = deepcopyjson(t.rotation)
        self.scale = deepcopyjson(t.scale)
        
    end
    function t:set()
        get_set_transform(set_lua,deepcopyjson(self))
    end
    function t:get_global_position()
        ret = Vec3:new(0,0,0)
        ret.x, ret.y,ret.z = get_global_position(self.object_ptr)
        return ret
    end
    function t:get_global_rotation()
        ret = Vec3:new(0,0,0)
        ret.x, ret.y,ret.z = get_global_rotation(self.object_ptr)
        return ret
    end
    function t:change_position(x,y,z)
        change_transfotm_position(self.object_ptr,x,y,z)
    end
    function t:change_rotation(x,y,z)
        change_transfotm_rotation(self.object_ptr,x,y,z)
    end
    function t:change_scale(x,y,z)
        change_transfotm_scale(self.object_ptr,x,y,z)
    end
    function t:get_local_direction(x,y,z)
        return get_local_direction_transform(self.object_ptr,x,y,z)
    end
    function t:get_global_position(x,y,z)
        return get_global_position_transform(self.object_ptr,x,y,z)
    end
    
    function t:get_translation_position(x,y,z)
        return get_translation_position_transform(self.object_ptr,x,y,z)
    end

    
    function t:delet()
        self.position = nil
        self.rotation = nil
        self.scale = nil
        self = nil
    end
    return t
end
component_map[components.transform] = transform_component
