#pragma once

#include <ostream>

#include <glm/glm.hpp>

/// @brief Class to represent a dimension which holds an independent "pixels" value
///        and a viewport size dependent "scale" value
class Dim {
public:
    /// @brief Default constructor
    /// @param pixels pixels
    /// @param scale scale
    Dim(int pixels = 0, float scale = 0.0f);

    /// @brief Creates an empty/zero-ed dimension
    /// @return dimension with zero values
    static Dim zero();

    /// @brief Constructor only for pixels
    /// @param pixels pixels
    /// @return Dim with specified pixels and zero scale
    static Dim fromPixels(int pixels);

    /// @brief Constructor only for scale
    /// @param scale scale
    /// @return Dim with specified scale and zero pixels
    static Dim fromScale(float scale);

    /// @brief Returns the maximum between two dimensions
    /// @param d0 first dimension
    /// @param d1 second dimension
    /// @return maximum of given dimensions
    static Dim max(const Dim &d0, const Dim &d1);

    /// @brief Returns the minimum between two dimensions
    /// @param d0 first dimension
    /// @param d1 second dimension
    /// @return minimum of given dimensions
    static Dim min(const Dim &d0, const Dim &d1);

    /// @brief Returns a dimension clamped to a min-max range
    /// @param d dimension to be clamped
    /// @param min minimum value
    /// @param max maximum value
    /// @return dimension clamped
    static Dim clamp(const Dim &d, const Dim &min, const Dim &max);

    Dim  operator+  (const Dim &d) const;
    Dim& operator+= (const Dim &d);

    Dim  operator-  () const;
    Dim  operator-  (const Dim &d) const;
    Dim& operator-= (const Dim &d);

    /// @brief Output operator
    /// @param os stream to send output to
    /// @param r dimension to output
    /// @return stream with outputted dimension
    friend std::ostream &operator<< (std::ostream &os, const Dim &r);

    /// @brief Get pixels
    /// @return pixels
    int pixels() const;

    /// @brief Get scale
    /// @return scale
    float scale() const;

    /// @brief Returns this dimension measured as scale
    /// @param dimensionSize max dimension size in pixels
    /// @return dimension in scale
    float toScale(float dimensionSize) const;

    /// @brief Returns this dimension measured as pixels
    /// @param dimensionSize max dimension size in pixels
    /// @return dimension in pixels
    float toPixels(float dimensionSize) const;

private:
    /// @brief Dimension in pixels, independent of viewport size
    int _pixels = 0;

    /// @brief Dimension in scale (usually in [0-1] interval), dependent of viewport size
    float _scale = 0.0f;
};

/// @brief Wrapper for a 2-dimensional Dim class
class Dim2 {
public:
    /// @brief Default constructor
    /// @param x dimension in x-axis
    /// @param y dimension in y-axis
    Dim2(const Dim &x, const Dim &y);

    /// @brief Constructor with separated params
    /// @param pixelsX pixels in x-axis
    /// @param scaleX scale in x-axis
    /// @param pixelsY pixels in y-axis
    /// @param scaleY scale in y-axis
    Dim2(
        int pixelsX = 0, float scaleX = 0.0f,
        int pixelsY = 0, float scaleY = 0.0f
    );

    /// @brief Creates an empty/zero-ed 2D dimension
    /// @return 2D dimension with zero values
    static Dim2 zero();

    /// @brief Constructor only for pixels
    /// @param x pixels in x-axis
    /// @param y pixels in y-axis
    /// @return Dim with specified pixels dimensions and zero scale
    static Dim2 fromPixels(int x, int y);

    /// @brief Constructor only for scale
    /// @param x scale in x-axis
    /// @param y scale in y-axis
    /// @return Dim with specified scale dimensions and zero pixels
    static Dim2 fromScale(float x, float y);

    /// @brief Returns the maximum between two 2D dimensions
    /// @param d0 first 2D dimension
    /// @param d1 second 2D dimension
    /// @return maximum of given 2D dimensions
    static Dim2 max(const Dim2 &d0, const Dim2 &d1);

    /// @brief Returns the minimum between two 2D dimensions
    /// @param d0 first 2D dimension
    /// @param d1 second 2D dimension
    /// @return minimum of given 2D dimensions
    static Dim2 min(const Dim2 &d0, const Dim2 &d1);

    /// @brief Returns a 2D dimension clamped to a min-max range
    /// @param d 2D dimension to be clamped
    /// @param min minimum value
    /// @param max maximum value
    /// @return 2D dimension clamped
    static Dim2 clamp(const Dim2 &d, const Dim2 &min, const Dim2 &max);

    Dim2  operator+  (const Dim2& d) const;
    Dim2& operator+= (const Dim2 &d);

    Dim2  operator-  () const;
    Dim2  operator-  (const Dim2 &d) const;
    Dim2& operator-= (const Dim2 &d);

    /// @brief Output operator
    /// @param os stream to send output to
    /// @param r 2D dimension to output
    /// @return stream with outputted 2D dimension
    friend std::ostream &operator<< (std::ostream &os, const Dim2 &r);

    /// @brief Get dimension in x-axis
    /// @return dimension in x-axis
    Dim x() const;

    /// @brief Get dimension in y-axis
    /// @return dimension in y-axis
    Dim y() const;

    /// @brief Returns this 2D dimension measured as scale
    /// @param windowSize render viewport size vector in pixels
    /// @return dimension in scale
    glm::vec2 toScale(const glm::vec2 &windowSize) const;

    /// @brief Returns this 2D dimension measured as pixels
    /// @param windowSize render viewport size vector in pixels
    /// @return dimension in pixels
    glm::vec2 toPixels(const glm::vec2 &windowSize) const;

private:
    /// @brief Dimension in x-axis
    Dim _x;

    /// @brief Dimension in y-axis
    Dim _y;
};
