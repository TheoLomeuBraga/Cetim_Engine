require("TMP_libs.components.component_index")


function get_set_physic_3D(get_set,object) 
end


--boady_dynamics
boady_dynamics = {}
boady_dynamics.static = 0
boady_dynamics.dynamic = 1

--colision_shapes
colision_shapes = {}
colision_shapes.cube = 0
colision_shapes.sphere = 1
colision_shapes.convex = 2


colision_layer_info = {}
function colision_layer_info:new()
    cli = {}
    cli.layer = 1
    cli.layers_can_colide = {1,}
    return cli
end

physics_3D_component = {}
function physics_3D_component:new(object_ptr)
    p = {}
    p.object_ptr = object_ptr
    function p:get()
        j = get_set_physic_3D(get_lua,self.object_ptr)
        
    end
    function p:set()

        get_set_physic_3D(set_lua,deepcopyjson(self))
    end
    function p:delet()
    end
    return p
end
component_map[components.physics_3D] = physics_3D_component