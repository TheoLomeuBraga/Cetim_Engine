require("TMP_libs.vectors")

matreial = {}
function matreial:new()
    m = {}
    m.shader = ""
    m.color = {}
    m.color.r = 1
    m.color.g = 1
    m.color.b = 1
    m.color.a = 1
    m.position_scale = Vec4:new(0,0,1,1)
    m.metallic = 0 
    m.softness = 0
    m.textures = {}
    m.inputs = {}
    function m:get(object,material_id)
        self.shader = get_shader(object,material_id)
        self.color.r,self.color.a,self.color.b,self.color.a = get_color(object,material_id)
        self.position_scale.x,self.position_scale.y,self.position_scale.z,self.position_scale.w = get_material_position_scale(object,material_id)
        i = 0
        while i < get_max_textures_material() do
            self.textures[i] = get_texture(object,material_id,i)
            i = i+1
        end
        i = 0
        while i < get_max_inputs_material() do
            self.inputs[i] = get_material_input(object,material_id,i)
            i = i+1
        end
    end
    function m:set(object,material_id)
        set_shader(object,material_id,self.shader)
        set_color(object,material_id,self.color.r,self.color.a,self.color.b,self.color.a)
        set_material_position_scale(object,material_id,self.position_scale.x,self.position_scale.y,self.position_scale.z,self.position_scale.w)
        i = 0
        while i < get_max_textures_material() do
            if self.textures[i] ~= nil then
                set_texture(object,material_id,i,self.textures[i])
            end
            
            i = i+1
        end
        i = 0
        while i < get_max_inputs_material() do
            if self.inputs[i] ~= nil then
                set_material_input(object,material_id,i,self.inputs[i])
            end
            
            i = i+1
        end
    end
    function m.delet()
        self.color = nil
        self.textures = nil
        self.inputs = nil
        self.position_scale = nil
        self = nil
    end
    return m
end