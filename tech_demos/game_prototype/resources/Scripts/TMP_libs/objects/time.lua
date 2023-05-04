Time = {}
Time.time = 0
Time.delta = 0
Time.scale = 0
function Time:set_speed(speed)
    set_time_scale(speed)
end
function Time:get()
    self.time, self.delta,self.sacale = get_time()
end