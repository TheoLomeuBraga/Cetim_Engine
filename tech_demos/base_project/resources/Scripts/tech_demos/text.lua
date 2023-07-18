require("TMP_libs.short_cuts.create_text")

local demo = {}

function demo:START(layers)
    mat = matreial:new()
    mat.shader = "resources/Shaders/text"
    mat.color = {r=0,g=0,b=1,a=1}
    create_text(layers.cenary,false,{x=0,y=0,z=0},{x=0,y=0,z=0},{x=0.1,y=0.1,z=0.1},mat,2,"hello world \nim Théo's game engine","resources/Fonts/Glowworm Regular.json")
end

function demo:UPDATE()

end

function demo:END()
    
end

return demo