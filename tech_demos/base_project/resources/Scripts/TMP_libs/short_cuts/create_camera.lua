require("TMP_libs.components.extras")
require("TMP_libs.objects.game_object")
require("TMP_libs.components.transform")
require("TMP_libs.components.camera")


function create_camera_perspective(father,pos,rot,zoom,ncf,fcp)
    ret = game_object:new(create_object(father)) 
    ret:add_component(components.transform)
    ret:add_component(components.camera)
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.camera].type = camera_view_types.perspective
    ret.components[components.camera].orthographc = false
    ret.components[components.camera].zoom = zoom
    ret.components[components.camera].ncp = ncf
    ret.components[components.camera].fcp = fcp
    ret.components[components.transform]:set()
    ret.components[components.camera]:set()
    return ret
end

function create_camera_ortho(father,pos,rot,scale_x,scale_y,res_x,res_y,ncf,fcp)
    ret = game_object:new(create_object(father)) 
    ret:add_component(components.transform)
    ret:add_component(components.camera)
    
    ret.components[components.transform].position = deepcopy(pos)
    ret.components[components.transform].rotation = deepcopy(rot)
    ret.components[components.camera].resolution = Vec2:new(res_x,res_y)
    ret.components[components.camera].size = Vec2:new(scale_x,scale_y)
    ret.components[components.camera].type = camera_view_types.ortho
    ret.components[components.camera].orthographc = true
    ret.components[components.camera].ncp = ncf
    ret.components[components.camera].fcp = fcp
    ret.components[components.transform]:set()
    ret.components[components.camera]:set()
    return ret
end
    
    