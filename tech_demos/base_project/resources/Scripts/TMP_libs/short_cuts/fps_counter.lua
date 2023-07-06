require("TMP_libs.components.extras")
require("TMP_libs.objects.game_object")
require("TMP_libs.components.transform")
require("TMP_libs.components.render_text")
require("TMP_libs.objects.time")
require("TMP_libs.short_cuts.create_text")
require("math")

function create_fps_counter(father,render_layer,font)
    local fps_counter_material = matreial:new()
    fps_counter_material.shader = "resources/Shaders/text"
    ret = create_text(father, true, Vec3:new(-0.95, 0.9, 0),Vec3:new(0.0, 0.0, 0.0),Vec3:new(0.05, 0.05, 0.05), fps_counter_material, render_layer, "", font)
    function ret:update_fps_cout()
        time:get()
        self.components[components.render_text].text = tostring(math.floor( 1 / time.delta ))
        self.components[components.render_text]:set()
    end
    return ret
end