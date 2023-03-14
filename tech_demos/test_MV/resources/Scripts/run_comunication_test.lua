require("TMP_libs.objects.game_object")
require("TMP_libs.components.lua_scripts")

test_object = {}

function create_test_object()
    test_object = game_object:new(create_object())
end

function run_test()
end

function destroy_test_object()
end

function full_test()
    print("runing comunication test")
    create_test_object()
    run_test()
    destroy_test_object()
end

