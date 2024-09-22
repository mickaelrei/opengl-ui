#pragma once

/// @brief Class to represent a dimension which holds an independent "pixel" value
///        and a viewport size dependent "scale" value
class Dim {
public:
    /// @brief Default constructor
    /// @param pixels pixels
    /// @param scale scale
    Dim(int pixels = 0, float scale = 1.0f);

    /// @brief Constructor only for pixels
    /// @param pixels pixels
    /// @return Dim with specified pixels and zero scale
    static Dim fromPixels(int pixels);

    /// @brief Constructor only for scale
    /// @param scale scale
    /// @return Dim with specified scale and zero pixels
    static Dim fromScale(float scale);

    Dim  operator+  (const Dim &d) const;
    Dim& operator+= (const Dim &d);

    Dim  operator-  () const;
    Dim  operator-  (const Dim &d) const;
    Dim& operator-= (const Dim &d);

    /// @brief Get pixels
    /// @return pixels
    int pixels() const;

    /// @brief Get scale
    /// @return scale
    float scale() const;

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

    /// @brief Constructor only for pixels
    /// @param x pixels in x-axis
    /// @param y pixels in y-axis
    /// @return Dim with specified pixel dimensions and zero scale
    static Dim2 fromPixels(int x, int y);

    /// @brief Constructor only for scale
    /// @param x scale in x-axis
    /// @param y scale in y-axis
    /// @return Dim with specified scale dimensions and zero pixels
    static Dim2 fromScale(float x, float y);

    Dim2  operator+  (const Dim2& d) const;
    Dim2& operator+= (const Dim2 &d);

    Dim2  operator-  () const;
    Dim2  operator-  (const Dim2 &d) const;
    Dim2& operator-= (const Dim2 &d);

    /// @brief Get dimension in x-axis
    /// @return dimension in x-axis
    Dim x() const;

    /// @brief Get dimension in y-axis
    /// @return dimension in y-axis
    Dim y() const;

private:
    /// @brief Dimension in x-axis
    Dim _x;

    /// @brief Dimension in y-axis
    Dim _y;
};
