register_function_set("physics")

require("components.base_component")
require("components.component_index")
require("objects.collision_shapes")

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

physics_2D_component = create_base_component(components.physics_2D)
physics_2D_component.scale = {x=1,y=1}
physics_2D_component.boady_dynamic = boady_dynamics.static
physics_2D_component.collision_shape = collision_shapes.tile
physics_2D_component.gravity_scale = 1
physics_2D_component.rotate = true
physics_2D_component.elasticity = 0
physics_2D_component.triger = false
physics_2D_component.friction = 1
physics_2D_component.density = 1
physics_2D_component.objs_touching = {}
physics_2D_component.colis_infos = {}
physics_2D_component.collision_layer = collision_layer_info:new()
physics_2D_component.vertex = {}

function physics_2D_component:clean()
    self = create_base_component(components.physics_2D)
    self.object_ptr = object_ptr
    self.scale = {x=1,y=1}
    self.boady_dynamic = boady_dynamics.static
    self.collision_shape = collision_shapes.tile
    self.gravity_scale = 1
    self.rotate = true
    self.elasticity = 0
    self.triger = false
    self.friction = 1
    self.density = 1
    self.objs_touching = {}
    self.colis_infos = {}
    self.collision_layer = collision_layer_info:new()
    self.vertex = {}
end

function physics_2D_component:get()
    local j = get_set_physic_2D(get_lua, self.object_ptr)
    self.scale = deepcopyjson(j.scale)
    self.boady_dynamic = j.boady_dynamic
    self.collision_shape = j.collision_shape
    self.gravity_scale = j.gravity_scale
    self.rotate = j.rotate > 0
    self.elasticity = j.elasticity
    self.triger = j.triger > 0
    self.friction = j.friction
    self.density = j.density
    self.objs_touching = deepcopyjson(j.objs_touching)
    self.collision_layer = deepcopyjson(j.collision_layer)
    self.colis_infos = deepcopyjson(j.colis_infos)
    self.vertex = deepcopyjson(j.vertex)
end

    function physics_2D_component:set()
        get_set_physic_2D(set_lua, deepcopyjson(self))
    end

    function physics_2D_component:to_move(speed_x, speed_y)
        to_move(self.object_ptr, speed_x, speed_y)
    end

    function physics_2D_component:add_force(force_x, force_y)
        add_force(self.object_ptr, force_x, force_y)
    end
    function physics_2D_component:add_impulse(force_x, force_y)
        add_impulse(self.object_ptr, force_x, force_y)
    end
    function physics_2D_component:set_linear_velocity(force_x, force_y)
        set_linear_velocity(self.object_ptr, force_x, force_y)
    end
    
    function physics_2D_component:add_rotative_force(force_x)
        add_rotative_force(self.object_ptr, force_x)
    end
    function physics_2D_component:add_rotative_impulse(force_x)
        add_rotative_impulse(self.object_ptr, force_x)
    end
    function physics_2D_component:set_angular_velocity(force_x)
        set_angular_velocity(self.object_ptr, force_x)
    end

component_map[components.physics_2D] = physics_2D_component:new(nil)
