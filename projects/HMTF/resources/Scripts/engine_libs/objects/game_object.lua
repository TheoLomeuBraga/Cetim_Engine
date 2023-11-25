register_function_set("game_object")

require("components.extras")
require("components.component_index")

function create_object(obj)
    if obj == nil then
        return c_create_object()
    end
    return c_create_object(obj)
end

function remove_object(obj)
    return c_remove_object(obj)
end

function add_component(obj, name)
    return c_add_component(obj, name)
end

function have_component(obj, name)
    return c_have_component(obj, name)
end

function remove_component(obj, name)
    return c_remove_component(obj, name)
end

function get_set_object(get_set, obj)
    return c_get_set_object(get_set, obj)
end

local componenta_meta_table = {
    __index = function(self, key)
        add_component(self.__object_ptr__, key)
        local ret = component_map[key]:new(self.__object_ptr__)
        rawset(self, key, ret)
        return ret
    end,
    __newindex = function(self, key, value)
        add_component(self.__object_ptr__, key)
        local ret = value
        rawset(self, key, ret)
        return ret
    end
}

game_object = {}
function game_object:new(object_ptr, not_recreate)
    local obj = {
        object_ptr = object_ptr,
        name = "",
        components = {
            __object_ptr__ = object_ptr,
        },
        father = "",
        children_ptr = {},
        children = {},
    }


    --deepprint(getmetatable(obj.components))
    setmetatable(obj.components, componenta_meta_table)

    function obj:add_component(component_name)
        return self.components[component_name]
    end

    function obj:remove_component(component_name)
        remove_component(self.object_ptr, component_name)
        self.components[component_name]:delet()
    end

    function obj:have_component(comp)
        return have_component(self.object_ptr, comp) == true
    end

    function obj:get_components()
        for key, value in pairs(components) do
            if self:have_component(value) then
                --self.components[value] = component_map[value].new(component_map[value],self.object_ptr)
                self.components[value] = component_map[value](component_map[value],self.object_ptr)
            end
        end
    end

    function obj:get()
        local j = get_set_object(lua_get, self.object_ptr)
        self.name = j.name
        self.father = j.father
        self.children_ptr = deepcopyjson(j.childrens)
        self:get_components()
    end

    function obj:set()
        get_set_object(lua_set, self)
    end

    if not_recreate ~= true then
        function obj:recreate()
            self:get()

            self.components = {
                __object_ptr__ = object_ptr,
            }
            setmetatable(self.components, componenta_meta_table)

            for i, v in pairs(components) do
                if self:have_component(v) then
                    self.components[v] = component_map[v]:new(self.object_ptr)
                    self.components[v]:get()
                end
            end
        end

        obj:recreate()
    end


    return obj
end
setmetatable(game_object,{
    __call = function (self,object_ptr, not_recreate)
        return self:new(object_ptr, not_recreate)
    end
})
