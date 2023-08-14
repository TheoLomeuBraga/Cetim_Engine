require("engine_libs.components.component_index")
require("engine_libs.objects.vectors")
require("engine_libs.functions")

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
transform_component.object_ptr = ""
transform_component.is_ui = false
transform_component.position = Vec3:new(0,0,0)
transform_component.rotation = Vec3:new(0,0,0)
transform_component.scale = Vec3:new(1,1,1)

function transform_component:get_global_position()
    local ret = Vec3:new(0,0,0)
    ret.x, ret.y,ret.z = get_global_position(self.object_ptr)
    return ret
end
function transform_component:get_global_rotation()
    local ret = Vec3:new(0,0,0)
    ret.x, ret.y,ret.z = get_global_rotation(self.object_ptr)
    return ret
end
function transform_component:change_position(x,y,z)
    change_transfotm_position(self.object_ptr,x,y,z)
end
function transform_component:change_rotation(x,y,z)
    change_transfotm_rotation(self.object_ptr,x,y,z)
end
function transform_component:change_scale(x,y,z)
    change_transfotm_scale(self.object_ptr,x,y,z)
end
function transform_component:get_local_direction(x,y,z)
    return get_local_direction_transform(self.object_ptr,x,y,z)
end
function transform_component:get_global_position(x,y,z)
    return get_global_position_transform(self.object_ptr,x,y,z)
end 
function transform_component:get_translation_position(x,y,z)
    return get_translation_position_transform(self.object_ptr,x,y,z)
end

function transform_component:set_master(object_ptr)    
    local ret = deepcopy(self)
    ret.object_ptr = object_ptr
    return ret
end



--component functions

function transform_component:get()
    local t = get_set_transform(get_lua,self.object_ptr)
    self.is_ui = t.is_ui > 0
    self.position = deepcopyjson(t.position)
    self.rotation = deepcopyjson(t.rotation)
    self.scale = deepcopyjson(t.scale)
        
end

function transform_component:set()
    get_set_transform(set_lua,deepcopyjson(self))
end

function transform_component:have(object_ptr)    
    return have_component(object_ptr, components.transform)
end

function transform_component:add(object_ptr)    
    add_component(object_ptr, components.transform)
end

function transform_component:remove(object_ptr)    
    remove_component(object_ptr,components.transform)
end

function transform_component:new(object_ptr)    
    local ret = deepcopy(self)
    ret.object_ptr = object_ptr
    return ret
end

component_map[components.transform] = transform_component:new(nil)