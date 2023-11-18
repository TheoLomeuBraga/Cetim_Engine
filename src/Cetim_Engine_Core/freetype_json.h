#pragma once
#include<map>
#include<vector>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class ft_char{
public:
    ft_char(){}
    wchar_t char_;
    int width;
    int height;
    int left;
    int top;
    int pitch;
    int adivancement;
    std::vector<char> bitmap;
};


class ft_font{
public:
    ft_font(){}
    bool pixel_perfect;
    std::map<wchar_t,ft_char> chars;
};


