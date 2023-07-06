require("TMP_libs.short_cuts.create_mesh")

function load_3D_asset(father, render_layer, object_3D)

    local ret = {}

    local object_type = object_3D.variables.type
    

    -- print("name",object_3D.name)

    if tablelength(object_3D.meshes) > 0 then

        -- local mesh_mat_size = math.min(tablelength(object_3D.meshes), tablelength(object_3D.materials))
        local materials = {}
        local i = 1
        for i, v in ipairs(object_3D.materials) do
            materials[i] = texture_dictionary(object_type)
            i = i + 1
        end

        -- print(object_3D.name, object_3D.position.x, object_3D.position.y, object_3D.position.x)

        ret = create_mesh(father, false, deepcopyjson(object_3D.position), deepcopyjson(object_3D.rotation),
            deepcopyjson(object_3D.scale), render_layer, materials, object_3D.meshes)

        ret.name = object_3D.name

        if object_type == "test_rb" then
            ret:add_component(components.physics_3D)
            ret.components[components.physics_3D].boady_dynamic = boady_dynamics.dynamic

            ret.components[components.physics_3D].collision_shape = collision_shapes.convex
            ret.components[components.physics_3D].collision_mesh = object_3D.meshes[1]

            ret.components[components.physics_3D]:set()
        elseif object_type == "test_sb" then
            ret:add_component(components.physics_3D)
            ret.components[components.physics_3D].boady_dynamic = boady_dynamics.static

            ret.components[components.physics_3D].collision_shape = collision_shapes.convex
            ret.components[components.physics_3D].collision_mesh = object_3D.meshes[1]

            ret.components[components.physics_3D]:set()
        end

    else
        if object_type == nil then
            ret = game_object:new(create_object(father))
            ret.name = object_3D.name
            ret:add_component(components.transform)
            ret.components[components.transform].position = deepcopyjson(object_3D.position)
            ret.components[components.transform].rotation = deepcopyjson(object_3D.rotation)
            ret.components[components.transform].scale = deepcopyjson(object_3D.scale)
            ret.components[components.transform]:set()
        elseif object_type == "player_start" then
            test_3D_game.camera = assets_from_map.player(test_3D_game.objects_layesrs.camera, deepcopyjson(object_3D.position), deepcopyjson(object_3D.rotation))
            --test_3D_game.camera = assets_from_map.free_camera(test_3D_game.objects_layesrs.camera, Vec3:new(-object_3D.position.x,object_3D.position.y,-object_3D.position.z), deepcopyjson(object_3D.rotation))
        end

    end

    for index, value in ipairs(object_3D.children) do
        ret.children[index] = self:object_3D_to_game_object(ret.object_ptr, render_layer, value)[1]
    end

    
    game_objects_in_cene_ret[object_3D.id] = deepcopy(ret)

    return {ret,deepcopy(game_objects_in_cene_ret)}
end