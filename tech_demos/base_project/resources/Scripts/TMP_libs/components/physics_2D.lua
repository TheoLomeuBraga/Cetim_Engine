require("TMP_libs.components.component_index")
require("TMP_libs.objects.collision_shapes")

function add_force(object, force_x, force_y)
end
function add_impulse(object, force_x, force_y)
end
function add_rotative_force(object, force)
end
function add_rotative_impulse(object, force)
end

function get_set_physic_2D(get_set, object)
end

function get_gravity()
end

function set_gravity(x, y, z)
end

function raycast_2D(position, target)
end

function set_linear_velocity(object,x,y)
    
end
function set_angular_velocity(object, force_x)
end

--boady_dynamics
boady_dynamics = {
    static = 0,
    dynamic = 1
}


--collision_shapes




collision_layer_info = {}
function collision_layer_info:new()
    cli = {}
    cli.layer = 1
    cli.layers_can_colide = { 1, }
    return cli
end

physics_2D_component = {}
function physics_2D_component:new(object_ptr)
    local p = {}
    p.object_ptr = object_ptr
    p.scale = Vec2:new(1, 1)
    p.boady_dynamic = boady_dynamics.static
    p.collision_shape = collision_shapes.tile
    p.gravity_scale = 1
    p.rotate = true
    p.triger = false
    p.friction = 1
    p.density = 1
    p.objs_touching = {}
    p.collision_layer = collision_layer_info:new()
    p.vertex = {}
    function p:get()
        j = get_set_physic_2D(get_lua, self.object_ptr)
        self.scale = deepcopyjson(j.scale)
        self.boady_dynamic = j.boady_dynamic
        self.collision_shape = j.collision_shape
        self.gravity_scale = j.gravity_scale
        self.rotate = j.rotate
        self.triger = j.triger
        self.friction = j.friction
        self.density = j.density
        self.objs_touching = deepcopyjson(j.objs_touching)
        self.collision_layer = deepcopyjson(j.collision_layer)
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
    function p:add_impulse(force_x, force_y)
        add_impulse(self.object_ptr, force_x, force_y)
    end
    function p:set_linear_velocity(force_x, force_y)
        set_linear_velocity(self.object_ptr, force_x, force_y)
    end
    
    function p:add_rotative_force(force_x)
        add_rotative_force(self.object_ptr, force_x)
    end
    function p:add_rotative_impulse(force_x)
        add_rotative_impulse(self.object_ptr, force_x)
    end
    function p:set_angular_velocity(force_x)
        set_angular_velocity(self.object_ptr, force_x)
    end
    

    function p:delet()
        self.scale = nil
        self.objs_touching = nil
        self = nil
    end

    return p
end

component_map[components.physics_2D] = physics_2D_component
