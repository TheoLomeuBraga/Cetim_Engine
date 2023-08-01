require("TMP_libs.short_cuts.create_text")

local demo = {}
demo.text1 = {}
demo.text2 = {}
demo.text3 = {}

function demo:START(layers)
    local mat = matreial:new()
    mat.shader = "resources/Shaders/text"
    mat.color = {r=0,g=0,b=1,a=1}
    demo.text1 = create_text(layers.cenary,false,{x=0,y=0,z=0},{x=0,y=0,z=0},{x=0.1,y=0.1,z=0.1},mat,2,"hello world \nim Théo's game engine","resources/Fonts/Glowworm Regular.json")

    mat.color = {r=0,g=1,b=0,a=1}
    demo.text2 = create_text(layers.cenary,false,{x=1,y=1,z=1},{x=0,y=0,z=0},{x=0.1,y=0.1,z=0.1},mat,2,"hello world \nim Théo's game engine","resources/Fonts/Glowworm Regular.json")

    mat.color = {r=1,g=0,b=0,a=1}
    demo.text3 = create_text(layers.cenary,false,{x=2,y=2,z=2},{x=0,y=0,z=0},{x=0.1,y=0.1,z=0.1},mat,2,"hello world \nim Théo's game engine","resources/Fonts/Glowworm Regular.json")
end

function demo:UPDATE()

end

function demo:END()
    remove_object(demo.text1.object_ptr)
    remove_object(demo.text2.object_ptr)
    remove_object(demo.text3.object_ptr)
    clear_memory()
end

return demo