#pragma once

#include <string>

#include <glm/glm.hpp>

#include "font.hpp"

/// @brief Class representing a text to be rendered on the screen
class Text {
public:
    Text() = default;

private:
    /// @brief The text to be rendered
    std::string text;

    /// @brief The font to use
    Font font;

    /// @brief Font size, character height in pixels
    float fontSize;

    /// @brief Text baseline start position in pixels
    glm::vec2 pos;

    // TODO: Add a ""
};