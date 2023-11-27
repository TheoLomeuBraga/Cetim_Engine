base_component = {}
base_component.component_name = ""
base_component.object_ptr = ""

function apply_component_metatable(component)
    setmetatable(component,{
        __call = function (self,object_ptr)
            add_component(object_ptr, self.component_name)
            return self:new(object_ptr)
        end
    })
    return component
end

function base_component:clean()
end

function base_component:get()
end
function base_component:set()
end
function base_component:new(object_ptr)
    self:clean()
    local ret = deepcopy(self)
    ret.object_ptr = object_ptr
    return apply_component_metatable(ret)
end
function base_component:have()    
    return have_component(self.object_ptr, self.component_name)
end

function base_component:add()    
    add_component(self.object_ptr, self.component_name)
end

function base_component:remove()    
    remove_component(self.object_ptr,self.component_name)
end

function create_base_component(component_name)
    local ret = base_component:new(nil)
    ret.component_name = component_name
    return ret
end