#pragma once

#include <string>

#include <glm/glm.hpp>

#include "freetype/ft2build.h"
#include FT_FREETYPE_H

// Redefine FreeType error string function
const char *FT_Error_String(FT_Error error)
{
    #undef FTERRORS_H_
    #define FT_ERROR_START_LIST     switch(error) {
    #define FT_ERRORDEF(e, v, s)    case e:\
                                        return s;
    #define FT_ERROR_END_LIST       default:\
                                        return "Unknown error"; \
                                    }
    #include "freetype/fterrors.h"
}

#define CHARS_START 32
#define CHARS_LEN 95

/// @brief Attempts to initialize resources related to fonts
/// @return whether was successful or not
bool initFonts();

/// @brief Terminates/frees resources related to fonts
void terminateFonts();

/// @brief Wrapper struct for FreeType glyph struct
struct Character {
    // OpenGL texture ID
    unsigned int textureID;

    // Size of glyph in pixels
    glm::ivec2 size;

    // Offset from baseline to top-left of glyph
    glm::ivec2 bearing;

    // Horizontal offset to advance to next glyph
    unsigned int advance;
};

/// @brief Class to hold 
class Font {
public:
    /// @brief Default constructor
    Font() = default;

    /// @brief Constructor with TTF file path
    /// @param ttfPath path to font file
    /// @param fontQuality font quality (texture height in pixels)
    Font(const std::string &ttfPath, unsigned int fontQuality = 48);

    /// @brief Get character info for a specific char
    /// @param c char
    /// @return character info for given char
    Character getCharInfo(char c);

    /// @brief Calculates text width as if it was written in a single horizontal line
    /// @param text text to calculate width
    /// @param fontSize font size in pixels
    /// @return text width in pixels
    unsigned int calculateTextWidth(const std::string &text, unsigned int fontSize = 14);

    /// @brief Returns internal FreeType Face pointer
    /// @return Internal FT_Face
    FT_Face getFreeTypeFace() const;

private:
    /// @brief FreeType face object
    FT_Face _face;

    /// @brief Array of all printable ASCII characters from 32 to 126
    Character _characters[CHARS_LEN];
};