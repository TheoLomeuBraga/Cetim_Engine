require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.short_cuts.create_ui")

local demo = {}

demo.menu = {}

demo.text1 = {}
demo.text2 = {}

demo.button_test = {}
demo.button_1 = {}
demo.button_2 = {}
demo.button_3 = {}

function print_hello_world()
    print("hello world")
end

function demo:START(layers)

    demo.menu = game_object:new(create_object(layers.hud))

    local font = "resources/Fonts/Glowworm Regular.json"
    local mat = matreial:new()

    mat.shader = "resources/Shaders/text"
    mat.color = {r=0,g=1,b=0,a=1}
    demo.text1 = create_text(demo.menu.object_ptr,true,{x=-1,y=0.8,z=0},{x=0,y=0,z=0},{x=0.1,y=0.1,z=0.1},mat,4,"just test the \nui buttons",font)

    
    --[[
    mat.color = {r=1,g=0,b=0,a=1}
    mat.shader = "resources/Shaders/button"
    mat.textures[1] = "resources/Textures/white.png"
    demo.button_test = create_button(demo.menu.object_ptr,true,{x=-1,y=1,z=0},{x=0,y=0,z=0},{x=1,y=1,z=1},4,mat)
    ]]

    
    local style = ui_style:new()
    style.color = {r=1,g=0,b=0,a=1}
    demo.button_1 = create_ui(demo.menu.object_ptr,true,{x=-1,y=0,z=0},{x=1,y=1,z=1},4,style,"hello world","resources/Textures/null.png",print_hello_world)

    --demo.button_2 = create_ui(demo.menu.object_ptr,true,{x=0,y=0,z=0},{x=1,y=1,z=1},4,style,"hello world","resources/Textures/null.png",print_hello_world)

    --demo.button_3 = create_ui(demo.menu.object_ptr,true,{x=0,y=0,z=0},{x=1,y=1,z=1},4,style,"hello world","resources/Textures/null.png",print_hello_world)
    
end

function demo:UPDATE()

end

function demo:END()
    remove_object(demo.menu.object_ptr)
    clear_memory()
end

return demo