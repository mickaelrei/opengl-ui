#include <stdexcept>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "glad/glad.h"

#include "debug.hpp"
#include "font.hpp"

/// @brief Global pointer to FreeType library object
static FT_Library ft;

/// @brief Map of currenty loaded fonts
static std::unordered_map<std::string, Font> loadedFonts;

/// @brief Whether font resources are already initialized
static bool initialized = false;

/// @brief Project root path
static std::string _rootPath = "";

const char *FT_Error_String(FT_Error error) {
    #undef FTERRORS_H_
    #define FT_ERROR_START_LIST     switch(error) {
    #define FT_ERRORDEF(e, v, s)    case e:\
                                        return s;
    #define FT_ERROR_END_LIST       default:\
                                        return "Unknown error"; \
                                    }
    #include "freetype/fterrors.h"
}

void FT_CheckError(const std::string &prefix, FT_Error code) {
    auto str = FT_Error_String(code);
    debugPrint("[FREETYPE] %s: %s\n", prefix.c_str(), str);
}

namespace FontModule {

bool init(const std::string &rootPath) {
    if (initialized) return true;
    initialized = true;

    _rootPath = rootPath;

    FT_Error error = FT_Init_FreeType(&ft);
    if (error != 0) {
        FT_CheckError("FT_Init_FreeType", error);
        return false;
    }

    // Adding an empty string at the end to suppress compiler warning
    debugPrint("Font module successfully loaded\n%s", "");
    return true;
}

void terminate() {
    if (!initialized) return;
    initialized = false;

    // Free loaded fonts
    for (auto &[ttfPath, font] : loadedFonts) {
        debugPrint("Freeing font at path %s\n", ttfPath.c_str());

        FT_Error err = FT_Done_Face(font.getFreeTypeFace());
        if (err != 0) FT_CheckError("FT_Done_Face", err);
        for (int i = 0; i < CHARS_LEN; ++i) {
            auto textureID = font.getCharInfo(i).textureID;
            glDeleteTextures(1, &textureID);
        }
    }

    // Free resources on FreeType library
    if (ft != nullptr) {
        FT_Error err = FT_Done_FreeType(ft);
        if (err != 0) FT_CheckError("FT_Done_Face", err);
    }
}

}

Font::Font(const std::string &ttfPath, float fontHeight)
  : _fontHeight{(float)fontHeight} {
    // Check if font is already loaded
    auto it = loadedFonts.find(ttfPath);
    if (it != loadedFonts.end()) {
        // Found, avoid reloading resources
        debugPrint("Font at path %s already loaded, using same resources\n", ttfPath.c_str());
        *this = it->second;
        return;
    }

    // Font not loaded yet, try loading resources
    debugPrint("Font at path %s not loaded yet, loading resources\n", ttfPath.c_str());

    // Trying to get ttf file to face struct
    FT_Error err = FT_New_Face(ft, (_rootPath + "/" + ttfPath).c_str(), 0, &_face);
    if (err != 0) {
        throw std::runtime_error{FT_Error_String(err)};
    }

    // Disable byte-alignment restriction
    // FT char texture will have a pixel every 1 byte (RED) as opposed to every 4 bytes (RGBA)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); glCheckError();

    // Generate characters from 32 to 126
    FT_Set_Pixel_Sizes(_face, 0, fontHeight);
    _maxCharHeight = 0.0f;
    _maxCharUnderflow = 0.0f;
    for (int i = 0; i < CHARS_LEN; ++i) {
        // Attempt to load char
        FT_Error err = FT_Load_Char(_face, CHARS_START + i, FT_LOAD_RENDER);
        if (err != 0) {
            debugPrint("FREETYPE: Failed to load char %c\n", i);
            FT_CheckError("FT_Load_Char", err);
            continue;
        }

        // Generate texture
        unsigned int texture;
        glGenTextures(1, &texture); glCheckError();
        glBindTexture(GL_TEXTURE_2D, texture); glCheckError();
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            _face->glyph->bitmap.width,
            _face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            _face->glyph->bitmap.buffer
        ); glCheckError();

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_CLAMP_TO_BORDER); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_CLAMP_TO_BORDER); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glCheckError();

        // Create character struct from glyph data
        Character character = {
            texture,
            glm::vec2{_face->glyph->bitmap.width, _face->glyph->bitmap.rows},
            glm::vec2{_face->glyph->bitmap_left, _face->glyph->bitmap_top},
            (float)(_face->glyph->advance.x >> 6)
        };
        _characters[i] = character;

        // Update font info
        _maxCharUnderflow = std::max(_maxCharUnderflow, character.size.y - character.bearing.y);
        _maxCharHeight = std::max(_maxCharHeight, character.size.y);
    }

    // Go back to original byte-alignment
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); glCheckError();

    // Store font in map
    loadedFonts[ttfPath] = *this;
}

Character Font::getCharInfo(char c) {
    // If not a printable character, space ' ' will be used
    if (c < 32 || c > 126) c = 32;

    return _characters[c - 32];
}

float Font::calculateTextWidth(const std::string &text, float fontSize) {
    const float scale = fontSize / fontHeight();
    const float textSize = text.size();

    float width = 0;
    for (size_t i = 0; i < textSize; ++i) {
        width += getCharInfo(text[i]).advance * scale;
    }
    return width;
}

FT_Face Font::getFreeTypeFace() const {
    return _face;
}

float Font::fontHeight() const {
    return _fontHeight;
}

float Font::maxCharHeight() const {
    return _maxCharHeight;
}

float Font::maxCharUnderflow() const {
    return _maxCharUnderflow;
}