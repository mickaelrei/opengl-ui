#pragma once

#include <glm/glm.hpp>

/// @brief Class used to build instances of BorderRadius
class Radius {
public:
    /// @brief Constructor for elliptical radius in pixels scale
    /// @param x radius on horizontal axis in pixels
    /// @param y radius on vertical axis in pixels
    /// @return elliptical radius in pixels scale
    static Radius ellipticalPixels(int x, int y);

    /// @brief Constructor for elliptical radius in [0-1] scale
    /// @param x radius on horizontal axis in [0-1]
    /// @param y radius on vertical axis in [0-1]
    /// @return elliptical radius in [0-1] scale
    static Radius ellipticalScale(float x, float y);

    /// @brief Constructor for circular radius in pixels scale
    /// @param r radius in pixels
    /// @return circular radius in pixels scale
    static Radius circularPixels(int r);

    /// @brief Constructor for circular radius in [0-1] scale
    /// @param r radius in [0-1]
    /// @return circular radius in [0-1] scale
    static Radius circularScale(float r);

    /// @brief Empty radius
    /// @return radius with no rounding
    static Radius zero();

    /// @brief Gets the horizontal radius
    /// @return horizontal radius in pixels
    int x() const;

    /// @brief Gets the vertical radius
    /// @return vertical radius in pixels
    int y() const;

    /// @brief Converts to a 2D vector
    /// @return 2D vector correspondent of the radii
    glm::vec2 toVector2() const;

    /// @brief Returns this radius measured from pixels to scale
    /// @param viewportSize viewport render size vector in pixels
    /// @return radius converted to [0-1] scale
    Radius toScale(const glm::vec2 &viewportSize) const;

    /// @brief Returns this radius measured from scale to pixels
    /// @param viewportSize viewport render size vector in pixels
    /// @return radius converted to pixels scale
    Radius toPixels(const glm::vec2 &viewportSize) const;

private:
    Radius(float x, float y, bool isPixels);

    /// @brief Radius on horizontal axis
    float _x;

    /// @brief Radius on vertical axis
    float _y;

    /// @brief Whether this radius is measured in pixels
    bool _isPixels;
};

/// @brief Class to use for defining border radiuses on UI elements
class BorderRadius {
public:
    /// @brief Constructor with 4 corners parameters
    /// @param topLeft radius on top left corner
    /// @param topRight radius on top right corner
    /// @param bottomLeft radius on bottom left corner
    /// @param bottomRight radius on bottom right corner
    BorderRadius(
        Radius topLeft = Radius::zero(),
        Radius topRight = Radius::zero(),
        Radius bottomLeft = Radius::zero(),
        Radius bottomRight = Radius::zero()
    );

    // TODO: Add pixels/scale constructors for each of these

    /// @brief Creates a border radius with same radii on all 4 corners
    /// @param radius radius
    /// @return border radius with equal corners
    static BorderRadius all(Radius radius);

    /// @brief Creates a circular border radius
    /// @param radius radius in pixels
    /// @return border radius with all 4 corners circularly rounded
    static BorderRadius circular(int radius);

    /// @brief Creates a horizontally symmetric border radius
    /// @param left radius on both left corners
    /// @param right radius on both right corners
    /// @return 
    static BorderRadius horizontal(
        Radius left = Radius::zero(),
        Radius right = Radius::zero()
    );

    /// @brief Creates a vertically symmetric border radius
    /// @param top radius on both top corners
    /// @param bottom radius on both bottom corners
    /// @return 
    static BorderRadius vertical(
        Radius top = Radius::zero(),
        Radius bottom = Radius::zero()
    );

    /// @brief Gets the top left radius
    /// @return top left radius
    Radius topLeft() const;

    /// @brief Gets the top right radius
    /// @return top right radius
    Radius topRight() const;

    /// @brief Gets the top left radius
    /// @return top left radius
    Radius bottomLeft() const;

    /// @brief Gets the bottom right radius
    /// @return bottom right radius
    Radius bottomRight() const;

    /// @brief Returns this border radius measured from pixels to scale
    /// @param viewportSize viewport render size vector in pixels
    /// @return border radius converted to [0-1] scale
    BorderRadius toScale(const glm::vec2 &viewportSize) const;

    /// @brief Returns this border radius measured from scale to pixels
    /// @param viewportSize viewport render size vector in pixels
    /// @return border radius converted to pixels scale
    BorderRadius toPixels(const glm::vec2 &viewportSize) const;

private:
    /// @brief Radius on top left corner
    Radius _topLeft;

    /// @brief Radius on top right corner
    Radius _topRight;

    /// @brief Radius on bottom left corner
    Radius _bottomLeft;

    /// @brief Radius on bottom right corner
    Radius _bottomRight;
};