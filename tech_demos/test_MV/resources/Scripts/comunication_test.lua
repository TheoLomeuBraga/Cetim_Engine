str = "hello"
num = 17
tab = {
    x = 1,
    y = 2,
    z = 3,
}

function mult_v3(arg)
    print(arg.x,arg.y,arg.z)
    ret = {
        x = arg.x * 2,
        y = arg.y * 2,
        z = arg.z * 2,
    }
    return ret
end

function START()
end

function UPDATE()
end

function COLIDE(Colision)
end

function END()
end