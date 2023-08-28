require("engine_libs.definitions")
require("engine_libs.components.extras")
require("engine_libs.components.component_all")
require("engine_libs.components.component_index")
require("engine_libs.objects.game_object")
require("engine_libs.objects.global_data")
require("engine_libs.short_cuts.create_ui")


mensage = "hello world"

interacting = false

function interact(args)

    global_data:set("pause",1)
    interacting = true

    print(mensage)

end

function START()
end

function UPDATE()
end

function COLLIDE(collision_info)
end

function END()
end
