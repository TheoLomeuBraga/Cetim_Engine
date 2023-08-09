animation = {}
function animation:new()
    a = {}
    a.time = 0
    a.duration = 0
    a.loop = true
    a.data = {}
    a.data.size = 0
    function a:reset()
        self.time = 0
    end
    function a:to_progress(speed)
        time:get()
        self.time = self.time + (time.delta * speed)
        if self.loop == true and self.time > self.duration then
            self.time = self.time - self.duration
        end
        if self.loop == false and self.time > self.duration then
            self.time = self.duration
        end
    end
    --this function may causes errors
    function a:get_frame()
        f = 0
        --duration  size
        --time      x
        f = self.data[math.floor((self.time * self.data.size) / self.duration)]
        return f
    end
    return a
end