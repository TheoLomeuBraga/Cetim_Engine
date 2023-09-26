register_function_set("render")

require("objects.material")

post_processing = {}
post_processing.material = material:new()
function post_processing:get()
    post_processing.material = deepcopy(get_set_post_processing(get_lua))
    print(post_processing.material)
end
function post_processing:set()
    get_set_post_processing(set_lua,post_processing.material)
end