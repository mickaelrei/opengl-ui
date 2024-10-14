#pragma once

#include <string>

#include <glm/glm.hpp>

#include "freetype/ft2build.h"
#include FT_FREETYPE_H

/// @brief Font character list start char
#define CHARS_START 32

/// @brief Font character list length
#define CHARS_LEN 95

/// @brief Namespace for font module
namespace FontModule {

/// @brief Attempts to initialize resources related to fonts
/// @param rootPath path to project root
/// @return whether was successful or not
bool init(const std::string &rootPath);

/// @brief Terminates/frees resources related to fonts
void terminate();

} // FontModule

/// @brief Wrapper struct for FreeType glyph struct
struct Character {
    /// @brief OpenGL texture ID
    unsigned int textureID;

    /// @brief Size of glyph in pixels
    glm::vec2 size;

    /// @brief Offset from baseline to top-left of glyph
    glm::vec2 bearing;

    /// @brief Horizontal offset to advance to next glyph
    float advance;
};

/// @brief Class to hold data about a TTF font
class Font {
public:
    /// @brief Default constructor
    Font() = default;

    /// @brief Constructor with TTF file path
    /// @param ttfPath path to font file
    /// @param fontHeight font height in pixels
    Font(const std::string &ttfPath, float fontHeight = 48.0f);

    /// @brief Get character info for a specific char
    /// @param c char
    /// @return character info for given char
    Character getCharInfo(char c);

    /// @brief Calculates text width as if it was written in a single horizontal line
    /// @param text text to calculate width
    /// @param fontSize font size in pixels
    /// @return text width in pixels
    float calculateTextWidth(const std::string &text, float fontSize = 14.0f);

    /// @brief Returns internal FreeType Face pointer
    /// @return Internal FT_Face
    FT_Face getFreeTypeFace() const;

    /// @brief At which height was this font loaded at
    /// @return Font height in pixels
    float fontHeight() const;

    /// @brief Height in pixels of tallest character in this font
    /// @return Max height in pixels
    float maxCharHeight() const;

    /// @brief Highest offset below baseline in pixels in this font
    /// @return Offset in pixels
    float maxCharUnderflow() const;

private:
    /// @brief FreeType face object
    FT_Face _face;

    /// @brief Font height in pixels
    float _fontHeight;

    /// @brief Height in pixels of tallest character
    float _maxCharHeight;

    /// @brief Highest offset below baseline in pixels
    float _maxCharUnderflow;

    /// @brief Array of all printable ASCII characters from 32 to 126
    Character _characters[CHARS_LEN];
};