register_function_set("time")

time = {}
time.time = 0
time.delta = 0
time.scale = 0
function time:set_speed(speed)
    set_time_scale(speed)
end
function time:get()
    self.time, self.delta,self.sacale = get_time()
end

stopwatch = {} -- A "classe" MyClass
function stopwatch:new()
    local obj = {
        creationTime = os.clock(),
    }
    function obj:getTime()
        return (os.clock() - self.creationTime)
    end
    return obj
end