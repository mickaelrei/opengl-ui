#pragma once

#include <vector>
#include <memory>

#include "shader.hpp"
#include "border_radius.hpp"
#include "dim.hpp"

/// @brief Namespace for quad module
namespace QuadModule {

/// @brief Attempts to initialize resources related to quad rendering
/// @param rootPath path to project root
/// @param windowSize initial window size
/// @return whether was successful or not
bool init(const std::string &rootPath, const glm::vec2 &windowSize);

/// @brief Terminates/frees resources related to quad rendering
void terminate();

/// @brief Callback for window resize
/// @param windowSize new window size in pixels
void onWindowResize(const glm::vec2 &windowSize);

} // QuadModule

/// @brief Class to represent a rectangular UI element
class Quad {
public:
    /// @brief Default constructor
    Quad() = default;

    /// @brief Constructor with window size
    /// @param windowSize current window size
    Quad(const glm::vec2 &windowSize);

    /// @brief Set new position
    /// @param pos position vector
    void setPosition(const Dim2 &pos);

    /// @brief Get position
    /// @return position vector
    Dim2 position() const;

    /// @brief Set new anchor point
    /// @param anchorPoint anchor point vector
    /// @note anchor point will be clamped to [0-1] range
    void setAnchorPoint(const glm::vec2 &anchorPoint);

    /// @brief Get anchor point
    /// @return anchor point
    glm::vec2 anchorPoint() const;

    /// @brief Set new size
    /// @param size size vector
    void setSize(const Dim2 &size);

    /// @brief Get size
    /// @return size vector
    Dim2 size() const;

    /// @brief Set new rotation
    /// @param rotation rotation radians
    void setRotation(float rotation);

    /// @brief Get rotation
    /// @return rotation in radians
    float rotation() const;

    /// @brief Set new color
    /// @param color color
    void setColor(const glm::vec4 &color);

    /// @brief Get color
    /// @return color
    glm::vec4 color() const;

    /// @brief Set new border radius
    /// @param borderRadius border radius
    void setBorderRadius(const BorderRadius &borderRadius);

    /// @brief Get border radius
    /// @return border radius
    BorderRadius borderRadius() const;

    /// @brief Adds a new child to this Quad
    /// @param child new child
    void addChild(const std::shared_ptr<Quad> &child);

    /// @brief Callback for when window is resized
    void onWindowResize(const glm::vec2 &windowSize);

    /// @brief Draws the quad
    /// @param windowSize window size in pixels
    /// @param model model matrix, used in recursive children rendering
    void draw(
        const glm::vec2 &windowSize,
        const glm::mat4 &model = glm::mat4{1.0f}
    );

private:
    /// @brief Sets needed uniforms on shader to draw the quad
    /// @param windowSize window size in pixels
    /// @param model model matrix
    void setUniforms(
        const glm::vec2 &windowSize,
        const glm::mat4 &model
    );

    /// @brief Calculates model matrix and stores for cached data
    void calculateModelMatrix();

    /// @brief Position
    Dim2 _pos;

    /// @brief Anchor point
    /// @note [0, 0] means _pos is taken as bottom left, [0.5, 0.5] as center and [1, 1] as top right
    glm::vec2 _anchorPoint = glm::vec2{0.5f};

    /// @brief Size
    Dim2 _size;

    /// @brief Rotation in radians
    float _rotation = 0.0f;

    /// @brief Color
    glm::vec4 _color = glm::vec4{1.0f};

    /// @brief Border radius
    BorderRadius _borderRadius;

    /// @brief Model matrix
    glm::mat4 _modelMatrix = glm::mat4{1.0f};

    /// @brief Last known window size
    glm::vec2 _lastWindowSize;

    /// @brief List of children
    std::vector<std::shared_ptr<Quad>> children;
};