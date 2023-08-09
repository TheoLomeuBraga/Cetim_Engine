require("TMP_libs.short_cuts.create_text")
require("TMP_libs.short_cuts.create_render_shader")
require("TMP_libs.short_cuts.create_ui")

local demo = {}

demo.menu = {}

demo.text1 = {}
demo.text2 = {}

demo.button_test = {}
demo.input_fild_1 = {}

demo.button_1 = {}
demo.button_2 = {}
demo.button_3 = {}

demo.display_1 = {}

local example_text = ""
function set_text(text)
    example_text = text
end

function ok()
    demo.display_1.text = example_text
end

function demo:START(layers)

    demo.menu = game_object:new(create_object(layers.hud))

    local font = "resources/Fonts/Glowworm Regular.json"
    local mat = matreial:new()

    --[[
    mat.color = {r=1,g=0,b=0,a=1}
    mat.shader = "resources/Shaders/button"
    mat.textures[1] = "resources/Textures/white.png"
    demo.button_test = create_render_shader(demo.menu.object_ptr,true,{x=-1,y=1,z=0},{x=0,y=0,z=0},{x=1,y=1,z=1},4,mat)
    ]]
    
    mat.shader = "resources/Shaders/text"
    mat.color = {r=0,g=1,b=0,a=1}
    demo.text1 = create_text(demo.menu.object_ptr,true,{x=0,y=0.8,z=0},{x=0,y=0,z=0},{x=0.05,y=0.05,z=0.05},mat,4,"just test the ui buttons\nhave fun",font)
    
    local style = ui_style:new()
    style.color = {r=1,g=0,b=0,a=1}
    style.color_hover = {r=0,g=1,b=0,a=1}
    style.color_click = {r=0,g=0,b=1,a=1}

    style.border_color = {r=0,g=0,b=0,a=0}
    style.border_color_hover = {r=0,g=0,b=1,a=1}
    style.border_color_click = {r=0,g=1,b=0,a=1}

    style.text_color = {r=0,g=1,b=1,a=1}

    demo.input_fild_1 = create_ui(demo.menu.object_ptr,{x=-1,y=0.7,z=0},{x=1.5,y=0.25,z=1},4,style,"click here to put some text",0.04,"resources/Textures/null.png",set_text,ui_category.input_fild)
    
    style.color = {r=0,g=1,b=0,a=1}
    style.color_hover = {r=1,g=0,b=0,a=1}
    demo.button_1 = create_ui(demo.menu.object_ptr,{x=0.5,y=0.7,z=0},{x=0.5,y=0.25,z=1},4,style,"ok",0.04,"resources/Textures/null.png",ok,ui_category.button)

    style.color = {r=0,g=0,b=0,a=1}
    demo.display_1 = create_ui(demo.menu.object_ptr,{x=-1,y=0.45,z=0},{x=2,y=0.25,z=1},4,style,"ok",0.04,"resources/Textures/null.png",ok,ui_category.display)
    
end

function demo:UPDATE()
    demo.input_fild_1:UPDATE()
    demo.button_1:UPDATE()
    demo.display_1:UPDATE()

    demo.input_fild_1.text = example_text
end

function demo:END()
    remove_object(demo.menu.object_ptr)
    clear_memory()
end

return demo