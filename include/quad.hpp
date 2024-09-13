#pragma once

#include "include/shader.hpp"
#include "include/border_radius.hpp"

/// @brief Class to represent a rectangular UI element
class Quad {
public:
    /// @brief Default constructor
    Quad();

    /// @brief Constructor with parameters
    /// @param pos position
    /// @param size size
    /// @param color color
    Quad(
        const glm::vec2 &pos,
        const glm::vec2 &size,
        const glm::vec4 &color = glm::vec4{1.0f}
    );

    /// @brief Set new size
    /// @param size size vector
    void setSize(const glm::vec2 &size);

    /// @brief Set new position
    /// @param pos position vector
    void setPosition(const glm::vec2 &pos);

    /// @brief Set new border radius
    /// @param borderRadius border radius
    void setBorderRadius(const BorderRadius &borderRadius);

    /// @brief Draws the quad
    /// @param shader shader to use
    /// @param windowSize window size in pixels
    void draw(const Shader &shader, const glm::vec2 &windowSize);

private:
    /// @brief Position
    glm::vec2 pos;

    /// @brief Size
    glm::vec2 size;

    /// @brief Color
    glm::vec4 color;

    /// @brief Border radius
    BorderRadius borderRadius;

    /// @brief OpenGL vertex array
    unsigned int VAO;

    /// @brief OpenGL vertex buffer
    unsigned int VBO;

    /// @brief OpenGL element buffer
    unsigned int EBO;
};