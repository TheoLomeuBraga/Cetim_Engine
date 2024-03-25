register_function_set("physics")

require("components.base_component")
require("components.component_index")
require("objects.collision_shapes")


--boady_dynamics
boady_dynamics = {
    static = 0,
    dynamic = 1,
    kinematic = 2,
}

--collision_shapes
--[[
    raycast_3D_ret = {
        collision_object = "",
        distance = 0,
        position = {x=0,y=0,z=0},
        speed = 0,
        normal = {x=0,y=0,z=0}
    }
]]
function raycast_3D(ray_start, ray_end)
    return c_raycast_3D(ray_start, ray_end)
end


collision_layer_info = {}
function collision_layer_info:new()
    return {
    layer = 1,
    layers_can_colide = { 1, }
    }
end



physics_3D_component = create_base_component(components.physics_3D)
physics_3D_component.object_ptr = nil
physics_3D_component.scale = { x = 1, y = 1, z = 1 }
physics_3D_component.boady_dynamic = boady_dynamics.static
physics_3D_component.collision_shape = collision_shapes.cube
physics_3D_component.rotate_x = true
physics_3D_component.rotate_y = true
physics_3D_component.rotate_z = true
physics_3D_component.elasticity = 0
physics_3D_component.triger = false
physics_3D_component.friction = 1
physics_3D_component.density = 1
physics_3D_component.collision_layer = collision_layer_info:new()
physics_3D_component.collision_mesh = { file = "", name = "" }
physics_3D_component.objs_touching = {}
physics_3D_component.colis_infos = {}
physics_3D_component.gravity_scale = 1
physics_3D_component.get_collision_info = false
function physics_3D_component:add_force(force_x, force_y,force_z)
    add_force(self.object_ptr, force_x, force_y,force_z)
end
function physics_3D_component:add_impulse(force_x, force_y,force_z)
    add_impulse(self.object_ptr, force_x, force_y,force_z)
end
function physics_3D_component:set_linear_velocity(force_x, force_y,force_z)
    set_linear_velocity(self.object_ptr, force_x, force_y,force_z)
end
function physics_3D_component:add_rotative_force(force_x, force_y,force_z)
    add_rotative_force(self.object_ptr, force_x, force_y,force_z)
end
function physics_3D_component:add_rotative_impulse(force_x, force_y,force_z)
    add_rotative_impulse(self.object_ptr, force_x, force_y,force_z)
end
function physics_3D_component:set_angular_velocity(force_x, force_y,force_z)
    set_angular_velocity(self.object_ptr, force_x,force_y,force_z)
end
function physics_3D_component:get_collision_infos()
    return get_collision_infos(self.object_ptr)
end
function physics_3D_component:get_objects_coliding()
    return get_objects_coliding(self.object_ptr)
end

function physics_3D_component:clean()
    self.scale = { x = 1, y = 1, z = 1 }
    self.boady_dynamic = boady_dynamics.static
    self.collision_shape = collision_shapes.cube
    self.rotate_x = true
    self.rotate_y = true
    self.rotate_z = true
    self.elasticity = 0
    self.triger = false
    self.friction = 1
    self.density = 1
    self.collision_layer = collision_layer_info:new()
    self.collision_mesh = { file = "", name = "" }
    self.objs_touching = {}
    self.colis_infos = {}
    self.gravity_scale = 1
    self.get_collision_info = false
end
function physics_3D_component:get()
    j = get_set_physic_3D(get_lua, self.object_ptr)
    self.scale = deepcopyjson(j.scale)
    self.boady_dynamic = j.boady_dynamic
    self.collision_shape = j.collision_shape
    self.rotate_x = j.rotate_x > 0
    self.rotate_y = j.rotate_y > 0
    self.rotate_z = j.rotate_z > 0
    self.elasticity = j.elasticity
    self.triger = j.triger > 0
    self.friction = j.friction
    self.density = j.density
    self.collision_layer = deepcopyjson(j.collision_layer)
    self.collision_mesh = deepcopyjson(j.collision_mesh)
    self.objs_touching = deepcopyjson(j.objs_touching)
    self.colis_infos = deepcopyjson(j.colis_infos)
    self.gravity_scale = j.gravity_scale
    self.get_collision_info = j.get_collision_info > 0
end
function physics_3D_component:set()
    get_set_physic_3D(set_lua, deepcopyjson(self))
end

component_map[components.physics_3D] = apply_component_metatable(physics_3D_component:new(nil))
