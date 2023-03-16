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

// function to get UTF-8 representation of a Unicode character code
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

std::string storeCharsInfoInJSON(const char* fontFilePath, const char* jsonFilePath,bool pixel_perfect) {
    
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
    
    /*
    ash to gpt:
    make a function to get UTF-8 representation of character code and  an c++ function that use fretype to print and storage all characters in utf-8 in a font file has including regional charters using an solution like "while (glyphIndex != 0)"  for the loop 
    */
    json fontInfo;
    std::vector<json> chars_infos;
    /*
    
    
    FT_ULong charCode;
    FT_UInt glyphIndex;
    std::vector<std::string> utf8Chars;
    charCode = FT_Get_First_Char(face, &glyphIndex);
    for (FT_ULong charCode = FT_Get_First_Char(face, NULL); charCode != 0; charCode = FT_Get_Next_Char(face, charCode, NULL)) {
        FT_UInt glyphIndex = FT_Get_Char_Index(face, charCode);

        
        std::cout << (char)charCode << std::endl;
        json charData = {
            {"char",(char)charCode},
            {"width",face->glyph->bitmap.width},
            {"height",face->glyph->bitmap.rows},
            {"left",face->glyph->bitmap_left},
            {"top",face->glyph->bitmap_top},
            {"pitch",face->glyph->bitmap.pitch},
        };

        std::vector<uint8_t> bitmapData;
        for (int y = 0; y < face->glyph->bitmap.rows; y++) {
            for (int x = 0; x < face->glyph->bitmap.width; x++) {
                bitmapData.push_back(face->glyph->bitmap.buffer[y * face->glyph->bitmap.pitch + x]);
            }
        }

        charData["bitmap"] = bitmapData;

        

        chars_infos.push_back(charData);
    }
    */
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
    std::cout << args[1] << " " << args[2] << " " << args[3] << std::endl;
    std::cout << storeCharsInfoInJSON(args[1].c_str(), args[2].c_str(),arg_true_false[args[3]]) << std::endl;
}