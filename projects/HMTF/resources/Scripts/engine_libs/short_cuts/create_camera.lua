require("components.extras")
require("objects.game_object")
require("components.transform")
require("components.camera")


function create_camera_perspective(father,pos,rot,zoom,ncf,fcp)
    ret = game_object(create_object(father)) 
    ret.components.transform.position = deepcopy(pos)
    ret.components.transform.rotation = deepcopy(rot)
    ret.components.camera.type = camera_view_types.perspective
    ret.components.camera.orthographc = false
    ret.components.camera.zoom = zoom
    ret.components.camera.ncp = ncf
    ret.components.camera.fcp = fcp
    ret.components.transform:set()
    ret.components.camera:set()
    return ret
end

function create_camera_ortho(father,pos,rot,scale_x,scale_y,res_x,res_y,ncf,fcp)
    ret = game_object(create_object(father)) 
    
    ret.components.transform.position = deepcopy(pos)
    ret.components.transform.rotation = deepcopy(rot)
    ret.components.camera.resolution = Vec2:new(res_x,res_y)
    ret.components.camera.size = Vec2:new(scale_x,scale_y)
    ret.components.camera.type = camera_view_types.ortho
    ret.components.camera.orthographc = true
    ret.components.camera.ncp = ncf
    ret.components.camera.fcp = fcp
    ret.components.transform:set()
    ret.components.camera:set()
    return ret
end
    
    