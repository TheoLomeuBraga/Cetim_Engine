require("TMP_libs.components.component_index")


function get_set_physic_3D(get_set, object)
end
function get_gravity()
end
function set_gravity(x,y,z)
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
    cli.layers_can_colide = { 1, }
    return cli
end

physics_3D_component = {}
function physics_3D_component:new(object_ptr)
    p = {}
    p.object_ptr = object_ptr
    p.scale = { x = 1, y = 1, z = 1 }
    p.boady_dynamic = boady_dynamics.static
    p.colision_shape = colision_shapes.cube
    p.rotate_X = true
    p.rotate_Y = true
    p.rotate_Z = true
    p.triger = false
    p.friction = 1
    p.density = 1
    p.colision_layer = colision_layer_info:new()
    p.colision_mesh = { file = "", name = "" };
    function p:get()
        j = get_set_physic_3D(get_lua, self.object_ptr)
        self.scale = deepcopyjson(j.scale)
        self.boady_dynamic = j.boady_dynamic
        self.colision_shape = j.colision_shape
        self.rotate_X = j.rotate_X
        self.rotate_Y = j.rotate_Y
        self.rotate_Z = j.rotate_Z
        self.triger = j.triger
        self.friction = j.friction
        self.density = j.density
        self.colision_layer = deepcopyjson(j.colision_layer)
        self.colision_mesh = deepcopyjson(j.colision_mesh)
    end

    function p:set()
        get_set_physic_3D(set_lua, deepcopyjson(self))
    end

    function p:delet()
    end

    return p
end

component_map[components.physics_3D] = physics_3D_component
