#pragma once

#include <ostream>

#include <glm/glm.hpp>

#include "dim.hpp"

/// @brief Class used to build instances of BorderRadius
class Radius {
public:
    /// @brief Constructor for elliptical radius
    /// @param r radius 2D dimension
    /// @return elliptical radius
    static Radius elliptical(const Dim2 &r);

    /// @brief Constructor for circular radius
    /// @param r radius dimension
    /// @return circular radius
    static Radius circular(const Dim &r);

    /// @brief Empty radius
    /// @return radius with no rounding
    static Radius zero();

    /// @brief Gets the horizontal radius
    /// @return horizontal radius in current scale
    Dim x() const;

    /// @brief Gets the vertical radius
    /// @return vertical radius in current scale
    Dim y() const;

    /// @brief Returns this radius measured as scale
    /// @param viewportSize viewport render size vector in pixels
    /// @return radius in scale
    glm::vec2 toScale(const glm::vec2 &viewportSize) const;

    /// @brief Returns this radius measured as pixels
    /// @param viewportSize viewport render size vector in pixels
    /// @return radius in pixels
    glm::vec2 toPixels(const glm::vec2 &viewportSize) const;

    /// @brief Output operator
    /// @param os stream to send output to
    /// @param r radius to output
    /// @return stream with outputted radius
    friend std::ostream &operator<< (std::ostream &os, const Radius &r);

private:
    /// @brief Default constructor
    /// @param x radius dimension on x-axis
    /// @param y radius dimension on y-axis
    Radius(const Dim &x, const Dim &y);

    /// @brief Radius dimension on x-axis
    Dim _x;

    /// @brief Radius dimension on y-axis
    Dim _y;
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

    /// @brief Creates a border radius with same radii on all 4 corners
    /// @param radius radius
    /// @return border radius with equal corners
    static BorderRadius all(Radius radius);

    /// @brief Creates a circular border radius
    /// @param radius radius
    /// @return border radius with all 4 corners circularly rounded
    static BorderRadius circular(const Dim &r);

    /// @brief Creates a horizontally symmetric border radius
    /// @param left radius on both left corners
    /// @param right radius on both right corners
    /// @return horizontally symmetric border radius
    static BorderRadius horizontal(
        Radius left = Radius::zero(),
        Radius right = Radius::zero()
    );

    /// @brief Creates a vertically symmetric border radius
    /// @param top radius on both top corners
    /// @param bottom radius on both bottom corners
    /// @return vertically symmetric border radius
    static BorderRadius vertical(
        Radius top = Radius::zero(),
        Radius bottom = Radius::zero()
    );

    /// @brief Empty border radius
    /// @return border radius with no rounding
    static BorderRadius zero();

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

    /// @brief Output operator
    /// @param os stream to send output to
    /// @param br border radius to output
    /// @return stream with outputted border radius
    friend std::ostream &operator<< (std::ostream &os, const BorderRadius &br);

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