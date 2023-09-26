register_function_set("gravity")

gravity = {force = {x = 0,y = -9.8, z = 0}}

function gravity:get()
    self.force.x,self.force.y,self.force.z = get_gravity()
end
function gravity:set()
    set_gravity(self.force.x,self.force.y,self.force.z)
end
