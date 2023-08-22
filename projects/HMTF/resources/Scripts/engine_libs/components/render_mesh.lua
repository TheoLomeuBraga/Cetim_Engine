require("engine_libs.components.base_component")
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



render_mesh_component = create_base_component(components.render_mesh)
render_mesh_component.layer = 2
render_mesh_component.use_oclusion = true
render_mesh_component.normal_direction = normal_direction.both
render_mesh_component.meshes = {}
render_mesh_component.materials = {}
function render_mesh_component:clean()
    self.layer = 2
    self.use_oclusion = true
    self.normal_direction = normal_direction.both
    self.meshes = {}
    self.materials = {}
end
function render_mesh_component:get()
    local j = get_set_render_mesh(get_lua,self.object_ptr)
    self.layer = j.layer
    self.use_oclusion = j.use_oclusion > 0
    self.normal_direction = j.normal_direction
    self.meshes = deepcopyjson(j.meshes) 
    self.materials = deepcopyjson(j.materials) 
end
function render_mesh_component:set()
    get_set_render_mesh(set_lua,deepcopyjson(self))
end
component_map[components.render_mesh] = render_mesh_component:new(nil)