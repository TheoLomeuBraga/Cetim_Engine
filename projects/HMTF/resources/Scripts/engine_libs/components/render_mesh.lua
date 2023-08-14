require("engine_libs.components.component_index")
require("engine_libs.components.extras")
require("engine_libs.objects.material")

--mesh
function get_mesh_json(object)
end
function set_mesh_json(object,json)
end

function get_set_render_mesh(get_set,object)
end


mesh_location = {}
function mesh_location:new(file,name)
    return {
        file = file,
        name = name
    }
end



render_mesh_component = {}
function render_mesh_component:new(object_ptr)
    local rm = {}
    rm.object_ptr = object_ptr
    rm.layer = 2
    rm.use_oclusion = true
    --rm.normal_direction = normal_direction.both
    rm.meshes = {}
    rm.materials = {}
    function rm:get()
        j = get_set_render_mesh(get_lua,self.object_ptr)
        self.layer = j.layer
        self.use_oclusion = j.use_oclusion > 0
        self.normal_direction = j.normal_direction
        self.meshes = deepcopyjson(j.meshes) 
        self.materials = deepcopyjson(j.materials) 
        
        

    end
    --buscar por erros
    function rm:set()
        get_set_render_mesh(set_lua,deepcopyjson(self))
    end
    return rm
end
component_map[components.render_mesh] = render_mesh_component