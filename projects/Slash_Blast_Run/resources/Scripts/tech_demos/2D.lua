require("TMP_libs.short_cuts.load_2D_map")
require("TMP_libs.short_cuts.create_collision")
require("TMP_libs.components.render_sprite")

json = require("libs.json")

local demo = {
    map_data = {},
    map_objects = {}
}

local this_layers = {}

function demo:load_objects(layer_data,tile_size_pixels)
    for key, value in pairs(layer_data.objects) do

        local selected_tile = 4
        local shape = collision_shapes.tile
        local vertex_data = {}
        local pos = Vec3:new(value.x / tile_size_pixels.x,-value.y / tile_size_pixels.y,0)
        local rot = Vec3:new(value.rotation,0,0)
        local sca = Vec3:new(1,1,1)
        local tile_set_local = "resources/Levels/2D/tile_set.json"
        local mat  = matreial:new()
        mat.shader = "resources/Shaders/sprite"
        local rigid_boady = true

        if value.name == "box" then

            selected_tile = 4
            shape = collision_shapes.box

        elseif value.name == "sphere" then

            selected_tile = 5
            shape = collision_shapes.sphere

        elseif value.name == "player_start" then

            selected_tile = 1
            tile_set_local = "resources/Sprites/chartes_2D/charter_2D.json"
            rigid_boady = true
            
        end

        local obj = create_collision_2D(demo.map_objects.object_ptr, pos, rot, sca, rigid_boady,shape,vertex_data,false)
        obj:add_component(components.render_sprite)
        obj.components[components.render_sprite].material = deepcopy(mat)
        obj.components[components.render_sprite].layer = 2
        obj.components[components.render_sprite].selected_tile = selected_tile
        obj.components[components.render_sprite].tile_set_local = tile_set_local
        obj.components[components.render_sprite]:set()

        if value.name == "player_start" then
            obj:add_component(components.lua_scripts)
            obj.components[components.lua_scripts]:add_script("game_scripts/charter_control")
            obj.components[components.lua_scripts]:set_variable("game_scripts/charter_control","charter_type","2D")
            obj.components[components.lua_scripts]:set_variable("game_scripts/charter_control","layers",this_layers)
            obj.components[components.lua_scripts]:set_variable("game_scripts/charter_control","charter_size",{x=1,y=1,z=1})
        end

    end
end

function demo:load_collision(layer_data,tile_size_pixels)
    for key, value in pairs(layer_data.objects) do

        local shape = collision_shapes.tiled_volume
        local vertex_data = {}

        if value.polyline ~= nil then
            local i = 1
            shape = collision_shapes.convex
            for index, value in ipairs(value.polyline) do
                vertex_data[i] = {x = value.x / tile_size_pixels.x, y = -value.y / tile_size_pixels.y}
                i = i + 1
            end
            vertex_data[tablelength(vertex_data)] = nil
        end

        local pos = Vec3:new(value.x / tile_size_pixels.x,-value.y / tile_size_pixels.y,0)
        local rot = Vec3:new(value.rotation,0,0)
        local sca = Vec3:new(value.width / tile_size_pixels.x,value.height / tile_size_pixels.y,1)

        local obj = create_collision_2D(demo.map_objects.object_ptr, pos, rot, sca, false,shape,vertex_data,false)

        

    end
end

function demo:START(layers)

    this_layers = deepcopy(layers)

    --"resources/Levels/2D/tile_map.json"
    local mat = matreial:new()
    mat.shader = "resources/Shaders/sprite"
    mat.color = {r=0,g=1,b=0,a=1}
    local data = load_2D_map(layers.cenary,{x=0,y=0,z=0},{x=0,y=0,z=0},{x=1,y=1,z=1},"resources/Levels/2D/tile_map.json","resources/Levels/2D/tile_set.json",mat)
    demo.map_data = deepcopy(data.tile_map_info)
    demo.map_objects = deepcopy(data.map_object)


    local tile_size_pixels = {x = demo.map_data.tilewidth,y =  demo.map_data.tileheight}
    for index, value in pairs(demo.map_data.layers) do
        if value.name == "objects" then
            demo:load_objects(value,tile_size_pixels)
        elseif value.name == "collision" then
            demo:load_collision(value,tile_size_pixels)
        end
    end

end

function demo:UPDATE()

end

function demo:END()
    remove_object(demo.map_objects.object_ptr)
    clear_memory()
end

return demo