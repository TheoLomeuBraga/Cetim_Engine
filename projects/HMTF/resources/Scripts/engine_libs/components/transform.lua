register_function_set("transform")

require("components.base_component")
require("components.component_index")
require("objects.vectors")
require("functions")



transform_component = create_base_component(components.transform)
transform_component.object_ptr = ""
transform_component.is_ui = false
transform_component.position = Vec3:new(0,0,0)
transform_component.rotation = Vec3:new(0,0,0)
transform_component.scale = Vec3:new(1,1,1)
transform_component.billboarding = 0



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
    if x == nil or y == nil or z == nil then
        return get_global_position_transform(self.object_ptr,0,0,0)
    end
    return get_global_position_transform(self.object_ptr,x,y,z)
end 
function transform_component:get_translation_position(x,y,z)
    return get_translation_position_transform(self.object_ptr,x,y,z)
end
function transform_component:look_at(target_position,look_up,look_axe)
    if look_axe == nil then
        look_axe = {x=0,y=1,z=0}
    end
    if look_up == nil then
        look_up = false
    end
    return c_transform_look_at(self.object_ptr,look_axe,target_position,look_up)
end


function transform_component:set_master(object_ptr)    
    local ret = (self)
    ret.object_ptr = object_ptr
    return ret
end



--component functions
function transform_component:clean()
    self.is_ui = false
    self.position = Vec3:new(0,0,0)
    self.rotation = Vec3:new(0,0,0)
    self.scale = Vec3:new(1,1,1)
    self.billboarding = 0
end

function transform_component:get()
    local t = get_set_transform(get_lua,self.object_ptr)
    self.is_ui = t.is_ui > 0
    self.position = deepcopyjson(t.position)
    self.rotation = deepcopyjson(t.rotation)
    self.scale = deepcopyjson(t.scale)
    self.billboarding = t.billboarding
        
end

function transform_component:set()
    get_set_transform(set_lua,deepcopyjson(self))
end



component_map[components.transform] = apply_component_metatable(transform_component:new(nil))