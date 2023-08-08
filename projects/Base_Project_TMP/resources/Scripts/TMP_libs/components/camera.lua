require("TMP_libs.components.component_index")

--camera
function set_camera(object, sizeX,sizeY,ncp,fcp)
end
function set_camera(object, zoom, resX,resY,ncp,fcp)
end
function get_camera(object)
end
function get_set_camera(get_set,object)
end

camera_component = {}
function camera_component:new(object_ptr)
    c = {}
    c.object_ptr = object_ptr
    c.type = ""
    c.orthographc = false
    c.size = Vec2:new(20,20)
    c.zoom = 90
    c.resolution = Vec2:new(100,100)
    c.fcp = 0
    c.ncp = 0
    function c:get()
        --[[
        self.type = get_camera(self.object_ptr)
        if self.type == camera_view_types.ortho then 
            self.type , self.size.x , self.size.y , self.ncp , self.fcp  = get_camera(self.object_ptr)
        end
        if self.type == camera_view_types.perspective then 
            self.type , self.zoom , self.resolution.x , self.resolution.y , self.ncp , self.fcp  = get_camera(self.object_ptr)
        end
        ]]
        j = get_set_camera(get_lua,self.object_ptr)
        self.orthographc = j.orthographc
        self.size = deepcopyjson(j.size)
        self.zoom = j.zoom
        self.resolution = deepcopyjson(j.resolution)
        self.fcp = j.fcp
        self.ncp = j.ncp
    end
    function c:set()
        --[[
        if self.type == camera_view_types.ortho then 
            set_camera(self.object_ptr, self.size.x,self.size.y,self.ncp,self.fcp)
        end
        if self.type == camera_view_types.perspective then 
            set_camera(self.object_ptr, self.zoom, self.resolution.x,self.resolution.y,self.ncp,self.fcp)
        end
        ]]
        get_set_camera(set_lua,deepcopyjson(self))
    end
    function c:delet()
        self.size = nil
        self.resolution = nil
        self = nil
    end
    return c
end
component_map[components.camera] = camera_component