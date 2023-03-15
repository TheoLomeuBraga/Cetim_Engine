require("TMP_libs.objects.game_object")
require("TMP_libs.components.lua_scripts")

test_object = {}

function create_test_object()
    test_object = game_object:new(create_object())
    test_object:add_component(components.lua_scripts)
    test_object.components[components.lua_scripts]:add_script("resources/Scripts/comunication_test.lua")
end

function run_test()
    print("valor z: ",test_object.components[components.lua_scripts]:get_variable("resources/Scripts/comunication_test.lua","tab").z)
    
    test_object.components[components.lua_scripts]:set_variable("resources/Scripts/comunication_test.lua","var_x",{x = 10,y = 20,z = 30})
    print("valor z: ",test_object.components[components.lua_scripts]:get_variable("resources/Scripts/comunication_test.lua","var_x").z)

    mlt_v3_ret = test_object.components[components.lua_scripts]:call_function("resources/Scripts/comunication_test.lua","mult_v3",{x = 10,y = 20,z = 30})
    print("retorno de mult_v3: ",mlt_v3_ret.x,mlt_v3_ret.y,mlt_v3_ret.z)
end

function destroy_test_object()
    remove_object(test_object.object_ptr)
end

function full_test()
    print("runing comunication test")
    create_test_object()
    run_test()
    destroy_test_object()
end

