require("TMP_libs.components.component_index")
require("TMP_libs.objects.collision_shapes")

function add_force(object, force_x, force_y)
end

function get_set_physic_2D(get_set, object)
end

function get_gravity()
end

function set_gravity(x, y, z)
end

function raycast_2D(position, distance, angle)
end

--boady_dynamics
boady_dynamics = {
    static = 0,
    dynamic = 1
}


--colision_shapes




colision_layer_info = {}
function colision_layer_info:new()
    cli = {}
    cli.layer = 1
    cli.layers_can_colide = { 1, }
    return cli
end

physics_2D_component = {}
function physics_2D_component:new(object_ptr)
    p = {}
    p.object_ptr = object_ptr
    p.scale = Vec2:new(1, 1)
    p.boady_dynamic = boady_dynamics.static
    p.colision_shape = colision_shapes.tile
    p.rotate = true
    p.triger = false
    p.friction = 1
    p.objects_coliding = {}
    p.colision_layer = colision_layer_info:new()
    p.vertex = {}
    function p:get()
        j = get_set_physic_2D(get_lua, self.object_ptr)
        self.scale = deepcopyjson(j.scale)
        self.boady_dynamic = j.boady_dynamic
        self.colision_shape = j.colision_shape
        self.rotate = j.rotate
        self.triger = j.triger
        self.friction = j.friction
        self.objects_coliding = deepcopyjson(j.objects_coliding)
        self.colision_layer = deepcopyjson(j.colision_layer)
        self.vertex = deepcopyjson(j.vertex)
    end

    function p:set()
        get_set_physic_2D(set_lua, deepcopyjson(self))
    end

    function p:to_move(speed_x, speed_y)
        to_move(self.object_ptr, speed_x, speed_y)
    end

    function p:add_force(force_x, force_y)
        add_force(self.object_ptr, force_x, force_y)
    end

    function p:delet()
        self.scale = nil
        self.objects_coliding = nil
        self = nil
    end

    return p
end

component_map[components.physics_2D] = physics_2D_component
