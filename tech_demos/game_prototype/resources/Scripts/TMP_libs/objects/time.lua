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