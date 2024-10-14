#pragma once

#include <string>

#include <glm/glm.hpp>

#include "font.hpp"

/// @brief Namespace for text module
namespace TextModule {

/// @brief Attempts to initialize resources related to text rendering
/// @param rootPath path to project root
/// @param windowSize initial window size
/// @return whether was successful or not
bool init(const std::string &rootPath, const glm::vec2 &windowSize);

/// @brief Terminates/frees resources related to text rendering
void terminate();

/// @brief Callback for window resize
/// @param windowSize new window size in pixels
void onWindowResize(const glm::vec2 &windowSize);

} // TextModule

/// @brief Enum for different types of text alignment
enum class TextAlignment {
    /// @brief Aligned to left
    left,

    /// @brief Aligned to right
    right,

    /// @brief Aligned to center
    center,

    /// @brief Justified
    justified
};

/// @brief Class representing a text to be rendered on the screen
class Text {
public:
    /// @brief Default constructor
    Text() = default;

    /// @brief Constructor
    /// @param text text to display
    /// @param font font to be used
    Text(const std::string &text, const Font &font);

    /// @brief Draws the quad
    /// @param windowSize window size vector in pixels
    void draw(const glm::vec2 &windowSize);

    /// @brief Set new text
    /// @param text text
    void setText(const std::string &text);

    /// @brief Get text
    /// @return text
    std::string text() const;

    /// @brief Set new font
    /// @param font font
    void setFont(const Font &font);

    /// @brief Get font
    /// @return font
    Font font() const;

    /// @brief Set new font size
    /// @param fontSize font size
    void setFontSize(float fontSize);

    /// @brief Get font size
    /// @return font size
    float fontSize() const;

    /// @brief Set new top left position vector
    /// @param topLeft top left position vector
    void setTopLeft(const glm::vec2 &topLeft);

    /// @brief Get top left position vector
    /// @return top left position vector
    glm::vec2 topLeft() const;

    /// @brief Set new render width
    /// @param renderWidth render width
    void setRenderWidth(float renderWidth);

    /// @brief Get render width
    /// @return render width
    float renderWidth() const;

    /// @brief Set new line height
    /// @param lineHeight line height
    void setLineHeight(float lineHeight);

    /// @brief Get line height
    /// @return line height
    float lineHeight() const;

    /// @brief Set new color
    /// @param color color
    void setColor(const glm::vec4 &color);

    /// @brief Get color
    /// @return color
    glm::vec4 color() const;

    /// @brief Set new text alignment
    /// @param alignment text alignment
    void setAlignment(TextAlignment alignment);

    /// @brief Get text alignment
    /// @return text alignment
    TextAlignment alignment() const;

private:
    /// @brief The text to be rendered
    std::string _text;

    /// @brief The font to use
    Font _font;

    /// @brief Font size, character height in pixels
    float _fontSize = 20.0f;

    /// @brief Text start top-left position in pixels
    glm::vec2 _topLeft = glm::vec2{0.0f};

    /// @brief How much the text extends horizontally in pixels
    float _renderWidth = 0.0f;

    /// @brief Text line height in font size scale (can't be negative)
    float _lineHeight = 1.2f;

    /// @brief Text color
    glm::vec4 _color = glm::vec4{1.0f};

    /// @brief Text alignment
    TextAlignment _alignment = TextAlignment::left;
};