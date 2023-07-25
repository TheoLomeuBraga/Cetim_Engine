require("TMP_libs.components.extras")
require("TMP_libs.components.component_all")
require("TMP_libs.components.component_index")
require("TMP_libs.objects.game_object")
require("TMP_libs.objects.input")
require("TMP_libs.objects.time")
require("TMP_libs.objects.global_data")
require("TMP_libs.objects.window")
require("math")

local this_object = {}

function START()
    this_object = game_object:new(this_object_ptr)
end

function UPDATE()

end

function END()
end

function COLLIDE(collision_info)
end