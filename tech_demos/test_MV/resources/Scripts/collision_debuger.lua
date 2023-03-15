function START()
end

function UPDATE()
end

function COLLIDE(collision_info)
    print("collision info {")
    for k, v in pairs(collision_info) do
        print(" ",k, v)
    end
    print("}")
end

function END()
end