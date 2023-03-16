#include <ft2build.h>
#include FT_FREETYPE_H
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <fstream>
#include <vector>
#include <iostream>

using json = nlohmann::json;

std::string storeCharsInfoInJSON(const char* fontFilePath, const char* jsonFilePath,bool pixel_perfect) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        // Error handling
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontFilePath, 0, &face)) {
        // Error handling
        FT_Done_FreeType(ft);
        return;
    }

    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE)) {
        // Error handling
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return;
    }

    json fontInfo;
    for (FT_ULong charCode = FT_Get_First_Char(face, NULL); charCode != 0; charCode = FT_Get_Next_Char(face, charCode, NULL)) {
        FT_UInt glyphIndex = FT_Get_Char_Index(face, charCode);
        if (glyphIndex == 0) {
            // Skip invalid characters
            continue;
        }

        if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER)) {
            // Error handling
            continue;
        }

        json charData;
        charData["char"] = (char)charCode;
        charData["width"] = face->glyph->bitmap.width;
        charData["height"] = face->glyph->bitmap.rows;
        charData["left"] = face->glyph->bitmap_left;
        charData["top"] = face->glyph->bitmap_top;

        std::vector<uint8_t> bitmapData;
        for (int y = 0; y < face->glyph->bitmap.rows; y++) {
            for (int x = 0; x < face->glyph->bitmap.width; x++) {
                bitmapData.push_back(face->glyph->bitmap.buffer[y * face->glyph->bitmap.pitch + x]);
            }
        }

        charData["bitmap"] = bitmapData;

        fontInfo["pixel_perfect"] = pixel_perfect;

        fontInfo.push_back(charData);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    std::ofstream outputFile(jsonFilePath);
    if (!outputFile.is_open()) {
        // Error handling
        return;
    }

    std::string fontInfo_dump = fontInfo.dump(4);

    std::cout << fontInfo_dump << std::endl;

    outputFile << fontInfo_dump; // Indent with 4 spaces
    outputFile.close();
}

std::vector<std::string> argumentos;

int main(int argc, char** argv)
{
	for (int i = 0; i < argc; i++) {
		argumentos.push_back(argv[i]);
	}
}