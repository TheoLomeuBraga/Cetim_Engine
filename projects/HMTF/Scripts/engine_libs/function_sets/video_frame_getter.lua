register_function_set("video")

function get_video_duration(path)
    return c_get_video_duration(path)
end

function load_video_frame(path,time)
    return c_load_video_frame(path,time)
end

function get_video_frame(path)
    return c_get_video_frame(path)
end