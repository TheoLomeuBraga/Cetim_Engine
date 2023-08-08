require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.short_cuts.create_ui")

local demo = {}

demo.text1 = {}
demo.text2 = {}

demo.button_1 = {}
demo.button_2 = {}
demo.button_3 = {}

function demo:START(layers)

    local font = "resources/Fonts/Glowworm Regular.json"
    local mat = matreial:new()
    
    

    mat.shader = "resources/Shaders/text"
    mat.color = {r=0,g=1,b=0,a=1}
    demo.text1 = create_text(layers.hud,true,{x=-1,y=0.8,z=0},{x=0,y=0,z=0},{x=0.1,y=0.1,z=0.1},mat,4,"just test the \nui buttons",font)

    
    mat.color = {r=1,g=0,b=0,a=1}
    mat.shader = "resources/Shaders/button"
    mat.textures[1] = "resources/Textures/white.png"
    demo.button_1 = create_button(layers.hud,true,{x=0,y=0,z=0},{x=0,y=0,z=0},{x=1,y=1,z=1},4,mat)
    
end

function demo:UPDATE()

end

function demo:END()
    remove_object(demo.text1.object_ptr)
    remove_object(demo.button_1.object_ptr)
    clear_memory()
end

return demo