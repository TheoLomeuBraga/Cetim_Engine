#include <ft2build.h>
#include FT_FREETYPE_H
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <fstream>
#include <vector>
#include <map>

#include <iostream>

#define AAAAA std::cout << "AAAAA\n";

bool fileExists(const char* filename) {
    std::ifstream file(filename);
    return file.good();
}

int stringToInt(const std::string& str) {
    std::istringstream iss(str);
    int result;
    iss >> result;
    return result;
}

// function to get UTF-8 representation of a Unicode character code
/*
std::string utf8_encode(unsigned int codepoint) {
    std::string result;
    if (codepoint <= 0x7F) {
        result.push_back((char)codepoint);
    } else if (codepoint <= 0x7FF) {
        result.push_back((char)(0xC0 | (codepoint >> 6)));
        result.push_back((char)(0x80 | (codepoint & 0x3F)));
    } else if (codepoint <= 0xFFFF) {
        result.push_back((char)(0xE0 | (codepoint >> 12)));
        result.push_back((char)(0x80 | ((codepoint >> 6) & 0x3F)));
        result.push_back((char)(0x80 | (codepoint & 0x3F)));
    } else if (codepoint <= 0x10FFFF) {
        result.push_back((char)(0xF0 | (codepoint >> 18)));
        result.push_back((char)(0x80 | ((codepoint >> 12) & 0x3F)));
        result.push_back((char)(0x80 | ((codepoint >> 6) & 0x3F)));
        result.push_back((char)(0x80 | (codepoint & 0x3F)));
    }
    return result;
}
*/

std::string storeCharsInfoInJSON(const char* fontFilePath, const char* jsonFilePath,bool pixel_perfect,int quality) {
    
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
	{
	    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}
    
    FT_Face face;
    if (FT_New_Face(ft, fontFilePath, 0, &face))
	{
	    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        if(!fileExists(fontFilePath)){
            std::cout << "ERROR::FREETYPE: file " << fontFilePath << " dont exist" << std::endl;
        }
	}
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
        // Error handling
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    FT_Set_Pixel_Sizes(face, 0, quality);

    json fontInfo;
    std::vector<json> chars_infos;

    
    FT_UInt glyphIndex;
    FT_ULong charCode = FT_Get_First_Char(face, &glyphIndex);
    while (glyphIndex != 0) {
        //std::string utf8Char = utf8_encode(charCode);
        //std::cout << utf8Char << " ";
        std::cout << (wchar_t)charCode << std::endl;
        FT_Load_Char(face, charCode, FT_LOAD_RENDER);
        json charData = {
            {"char",(wchar_t)charCode},
            {"width",face->glyph->bitmap.width},
            {"height",face->glyph->bitmap.rows},
            {"left",face->glyph->bitmap_left},
            {"top",face->glyph->bitmap_top},
            {"pitch",face->glyph->bitmap.pitch},
            {"adivancement",(float)face->glyph->advance.x / 64},
        };

        std::vector<uint8_t> bitmapData;
        for (int i = 0; i < face->glyph->bitmap.rows * face->glyph->bitmap.width; i++){
            bitmapData.push_back(face->glyph->bitmap.buffer[i]);
        }

        charData["bitmap"] = bitmapData;

        chars_infos.push_back(charData);

        charCode = FT_Get_Next_Char(face, charCode, &glyphIndex);
    }

    fontInfo["pixel_perfect"] = pixel_perfect;
    fontInfo["chars"] = chars_infos;

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    std::ofstream outputFile(jsonFilePath);

    
    std::string fontInfo_dump = fontInfo.dump();

    outputFile << fontInfo_dump; // Indent with 4 spaces
    outputFile.close();
    return fontInfo_dump;
}




int main(int argc, char** argv)
{
    std::vector<std::string> args;

	for (int i = 0; i < argc; i++) {
		args.push_back(argv[i]);
	}
    
    std::map<std::string,bool> arg_true_false = {
        std::pair<std::string,bool>("0",false),
        std::pair<std::string,bool>("1",true),

        std::pair<std::string,bool>("false",false),
        std::pair<std::string,bool>("true",true),

        std::pair<std::string,bool>("False",false),
        std::pair<std::string,bool>("True",true),
    }; 
    std::cout << "args: " << args[1] << " " << args[2] << " " << args[3] << " " << args[4] << std::endl;
    std::cout << storeCharsInfoInJSON(args[1].c_str(), args[2].c_str(),arg_true_false[args[3]],stringToInt(args[4])) << std::endl;
}