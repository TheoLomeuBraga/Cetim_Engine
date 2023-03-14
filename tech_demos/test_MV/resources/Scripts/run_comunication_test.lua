require("TMP_libs.objects.game_object")
require("TMP_libs.components.lua_scripts")

test_object = {}

function create_test_object()
    test_object = game_object:new(create_object())
    test_object:add_component(components.lua_scripts)
    test_object.components[components.lua_scripts]:add_script("resources/Scripts/comunication_test.lua")
end

function run_test()
    print("valor z: ",test_object.components[components.lua_scripts]:get_variable(test_object.object_ptr,"resources/Scripts/comunication_test.lua","tab").z)
end

function destroy_test_object()
end

function full_test()
    print("runing comunication test")
    create_test_object()
    run_test()
    --destroy_test_object()
end

