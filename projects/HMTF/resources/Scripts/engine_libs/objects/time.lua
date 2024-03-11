register_function_set("time")

time = {}
time.time = 0
time.delta = 0
time.scale = 0
function time:set_speed(speed)
    c_set_time_scale(speed)
end
function time:get()
    local t = c_get_time()
    time.time = t.time
    time.delta = t.delta
    time.scale = t.scale
end

stopwatch = {} -- A "classe" MyClass
function stopwatch:new()
    local obj = {
        creationTime = os.clock(),
    }
    function obj:get()
        return (os.clock() - self.creationTime)
    end
    return obj
end