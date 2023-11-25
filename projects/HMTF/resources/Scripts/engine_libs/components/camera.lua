register_function_set("camera")

require("components.base_component")
require("components.component_index")

--camera


camera_component = create_base_component(components.camera)
camera_component.object_ptr = ""
camera_component.type = ""
camera_component.orthographc = false
camera_component.size = {x = 20,y = 20}
camera_component.zoom = 90
camera_component.resolution = {x = 100,y = 100}
camera_component.fcp = 100
camera_component.ncp = 0.01


function camera_component:clean()
    self.type = ""
    self.orthographc = false
    self.size = {x = 20,y = 20}
    self.zoom = 90
    self.resolution = {x = 100,y = 100}
    self.fcp = 100
    self.ncp = 0.01
end

function camera_component:get()
    local j = get_set_camera(get_lua,self.object_ptr)
    self.orthographc = j.orthographc > 0
    self.size = deepcopyjson(j.size)
    self.zoom = j.zoom
    self.resolution = deepcopyjson(j.resolution)
    self.fcp = j.fcp
    self.ncp = j.ncp
end
function camera_component:set()
    get_set_camera(set_lua,deepcopyjson(self))
end
component_map[components.camera] = apply_component_metatable(camera_component:new(nil))