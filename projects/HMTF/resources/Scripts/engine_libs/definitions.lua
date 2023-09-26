argsn = 0
args = {}

function table.shallow_copy(t)
    local t2 = {}
    for k,v in pairs(t) do
      t2[k] = v
    end
    return t2
end

function shallow_copy(t)
    return table.shallow_copy(t)
end


--camera types
camera_view_types = {}
camera_view_types.perspective = "perspective"
camera_view_types.ortho = "ortho"

--get set
get_lua = 0
set_lua = 1