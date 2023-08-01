json = require "libs.json"

--assets

tile = {}
function tile:new()
    t = {}
    t.id = 0
	t.name = ""
	t.collision = 0
	t.visible = true
	t.interactible = false
	t.script = ""
    t.comment = ""
    function t:get(path,image_folder,tile_id)
        self.id  , self.name , self.collision , self.visible, self.interactible , self.script,self.comment = get_tile_set_tile(path,image_folder,tile_id -1)
    end
    return t
end

tile_map = {}
function tile_map:new()
    tm = {}
    tm.name = ""
    tm.size_x = 0
    tm.size_y = 0
    tm.layers = 0
    tm.data = {}
    function tm:get(path)
        self.name = path
        self.size_x,self.size_y = get_tilemap_size(path)
        self.layers = get_tilemap_layer_size(path)
        layer_id = 0
        tile_id = 0
        i = 0
        while layer_id < self.layers do
            while tile_id < (self.size_x * self.size_y) do
                self.data[i] = get_tilemap_data(path,layer_id,tile_id)
                i = i + 1
                tile_id = tile_id + 1
            end
            layer_id = layer_id + 1
        end
    end
    function tm:get_tile_id_from(layer,id)
        return self.data[(self.size_x * self.size_y * self.layers) +  id]
    end
    function tm:get_tile_id_from(layer,x,y)
        return self.data[(self.size_x * self.size_y * self.layers) +  (y * self.size_x) + x]
    end

    return tm
end




scene_3D = {}
function scene_3D:new()
    ret = {}
    ret.name = ""
    ret.meshs = {}
    ret.materials = {}
    ret.textures = {}
    ret.objects = {}
    ret.animation = {}
    function ret:get(path)
        --[[
        scene_3D_json = get_scene_3D_json(path)
        print(scene_3D_json)
        scene_3D = json.decode(scene_3D_json)
        ]]
        scene_3D = json.decode(get_scene_3D_json(path))
        self.name = path
        self.meshs = scene_3D.meshs
        self.materials = scene_3D.materials
        self.textures = scene_3D.textures
        self.objects = scene_3D.objects
        self.animation = nil
        
    end
    return ret
end





