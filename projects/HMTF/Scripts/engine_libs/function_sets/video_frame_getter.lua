register_function_set("video")

function load_video_frame(path,time)
    return c_load_video_frame(path,time)
end

function get_video_frame(path)
    return c_get_video_frame(path)
end