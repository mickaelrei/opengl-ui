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

    /// @brief Set new position
    /// @param pos position vector
    void setPosition(const glm::vec2 &pos);

    /// @brief Get position
    /// @return position vector
    glm::vec2 position() const;

    /// @brief Set new size
    /// @param size size vector
    void setSize(const glm::vec2 &size);

    /// @brief Get size
    /// @return size vector
    glm::vec2 size() const;

    /// @brief Set new rotation
    /// @param rotation rotation radians
    void setRotation(float rotation);

    /// @brief Get rotation
    /// @return rotation in radians
    float rotation() const;

    /// @brief Set new border radius
    /// @param borderRadius border radius
    void setBorderRadius(const BorderRadius &borderRadius);

    /// @brief Get border radius
    /// @return border radius
    BorderRadius borderRadius() const;

    /// @brief Draws the quad
    /// @param shader shader to use
    /// @param windowSize window size in pixels
    void draw(const Shader &shader, const glm::vec2 &windowSize);

private:
    /// @brief Position
    glm::vec2 _pos = glm::vec2{0.0f};

    /// @brief Size
    glm::vec2 _size = glm::vec2{0.0f};

    /// @brief Rotation in radians
    float _rotation = 0.0f;

    /// @brief Color
    glm::vec4 _color = glm::vec4{1.0f};

    /// @brief Border radius
    BorderRadius _borderRadius;

    /// @brief OpenGL vertex array
    unsigned int VAO;

    /// @brief OpenGL vertex buffer
    unsigned int VBO;

    /// @brief OpenGL element buffer
    unsigned int EBO;
};