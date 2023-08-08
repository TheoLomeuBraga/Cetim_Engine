require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.short_cuts.create_button")

local demo = {}

demo.text1 = {}

demo.button_1 = {}
demo.button_2 = {}
demo.button_3 = {}

function demo:START(layers)
    local mat = matreial:new()
    mat.shader = "resources/Shaders/text"
    mat.color = {r=0,g=1,b=0,a=1}
    demo.text1 = create_text(layers.cenary,false,{x=0,y=0,z=-10},{x=0,y=0,z=0},{x=0.5,y=0.5,z=0.5},mat,2,"just test the buttons","resources/Fonts/Glowworm Regular.json")

    mat.color = {r=1,g=0,b=0,a=1}
    mat.shader = "resources/Shaders/button"
    mat.textures[1] = "resources/Textures/white.png"
    create_button(layers.hud,true,{x=0,y=0,z=1},{x=0,y=45,z=0},{x=1,y=1,z=1},3,mat)
    
end

function demo:UPDATE()

end

function demo:END()
    remove_object(demo.text1.object_ptr)
    clear_memory()
end

return demo