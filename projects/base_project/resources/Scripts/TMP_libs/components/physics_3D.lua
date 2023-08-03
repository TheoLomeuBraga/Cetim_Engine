require("TMP_libs.components.component_index")
require("TMP_libs.objects.collision_shapes")

function get_set_physic_3D(get_set, object)
end

function add_force(object, force_x, force_y,force_z)
end
function add_impulse(object, force_x, force_y,force_z)
end

function add_rotative_force(object, force_x, force_y,force_z)
end
function add_rotative_impulse(object, force_x, force_y,force_z)
end

function set_linear_velocity(object,x,y,z)
end
function set_angular_velocity(object, force_x,force_y,force_z)
end

function get_gravity()
end
function set_gravity(object,x, y, z)
end

function raycast_3D(ray_from,ray_to)
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

physics_3D_component = {}
function physics_3D_component:new(object_ptr)
    p = {}
    p.object_ptr = object_ptr
    p.scale = { x = 1, y = 1, z = 1 }
    p.boady_dynamic = boady_dynamics.static
    p.collision_shape = collision_shapes.cube
    p.rotate_X = 1
    p.rotate_Y = 1
    p.rotate_Z = 1
    p.triger = 0
    p.friction = 1
    p.density = 1
    p.collision_layer = collision_layer_info:new()
    p.collision_mesh = { file = "", name = "" }
    p.objs_touching = {}
    function p:get()
        j = get_set_physic_3D(get_lua, self.object_ptr)
        self.scale = deepcopyjson(j.scale)
        self.boady_dynamic = j.boady_dynamic
        self.collision_shape = j.collision_shape
        self.rotate_X = j.rotate_X
        self.rotate_Y = j.rotate_Y
        self.rotate_Z = j.rotate_Z
        self.triger = j.triger
        self.friction = j.friction
        self.density = j.density
        self.collision_layer = deepcopyjson(j.collision_layer)
        self.collision_mesh = deepcopyjson(j.collision_mesh)
        self.objs_touching = deepcopyjson(j.objs_touching)
    end
    

    function p:set()
        get_set_physic_3D(set_lua, deepcopyjson(self))
    end

    function p:add_force(force_x, force_y,force_z)
        add_force(self.object_ptr, force_x, force_y,force_z)
    end
    function p:add_impulse(force_x, force_y,force_z)
        add_impulse(self.object_ptr, force_x, force_y,force_z)
    end
    function p:set_linear_velocity(force_x, force_y,force_z)
        set_linear_velocity(self.object_ptr, force_x, force_y,force_z)
    end
    function p:add_rotative_force(force_x, force_y,force_z)
        add_rotative_force(self.object_ptr, force_x, force_y,force_z)
    end
    function p:add_rotative_impulse(force_x, force_y,force_z)
        add_rotative_impulse(self.object_ptr, force_x, force_y,force_z)
    end
    function p:set_angular_velocity(force_x, force_y,force_z)
        set_angular_velocity(self.object_ptr, force_x,force_y,force_z)
    end
    

    function p:delet()
    end

    return p
end

component_map[components.physics_3D] = physics_3D_component
