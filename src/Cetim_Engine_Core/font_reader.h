#include <sstream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <fstream>
#include <vector>
#include <map>
#include <string>
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
        //std::cout << (wchar_t)charCode << std::endl;
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
    fontInfo["quality"] = quality;

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    std::ofstream outputFile(jsonFilePath);

    
    std::string fontInfo_dump = fontInfo.dump();

    outputFile << fontInfo_dump; // Indent with 4 spaces
    outputFile.close();
    return fontInfo_dump;
}