require("TMP_libs.components.extras")
require("TMP_libs.components.component_index")


--object

function create_object()
end

function create_object(parent)
end

function get_object_with_name(name)
end

function remove_object(object)
end

function have_component(object, component)
end

function add_component(object, component)
end

function remove_component(object, component)
end

--function get_object_family(object)
--end

function get_set_object(get_set,object)
end

game_object = {}
function game_object:new(object_ptr)
    obj = {}
    obj.object_ptr = object_ptr
    obj.name = ""
    obj.components = {}
    obj.father = ""
    obj.children_ptr = {}
    obj.children = {}
    function obj:add_component(component_name)
        add_component(self.object_ptr, component_name)
        self.components[component_name] = component_map[component_name]:new(self.object_ptr)
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
                self.components[value] = component_map[value]:new(self.object_ptr)
            end
        end
    end

    function obj:get()
        local j = get_set_object(lua_get,self.object_ptr)
        self.name = j.name
        self.father = j.father
        self.children_ptr = deepcopyjson(j.childrens)
    end

    
    function obj:set()
        get_set_object(lua_set,self)
    end
    

    function obj:recreate()
        self:get()

        self.components = {}

        for i, v in pairs(components) do
            if self:have_component(v) then
                self.components[v] = component_map[v]:new(self.object_ptr)
                self.components[v]:get()
            end
        end
    end

    obj:recreate()

    return obj
end
