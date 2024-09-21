#include <algorithm>

#include "border_radius.hpp"

Radius::Radius(
    float x,
    float y,
    bool isPixels
) : _x{x}, _y{y}, _isPixels{isPixels} {}

Radius Radius::ellipticalPixel(int x, int y) {
    int _x = std::max(0, x);
    int _y = std::max(0, y);
    return Radius{static_cast<float>(_x), static_cast<float>(_y), true};
}

Radius Radius::ellipticalScale(float x, float y) {
    float _x = std::clamp(x, 0.0f, 1.0f);
    float _y = std::clamp(y, 0.0f, 1.0f);
    return Radius{_x, _y, false};
}

Radius Radius::circularPixel(int r) {
    float _r = static_cast<float>(r);
    _r = std::max(_r, 0.0f);
    return Radius{_r, _r, true};
}

Radius Radius::circularScale(float r) {
    float _r = std::clamp(r, 0.0f, 1.0f);
    return Radius{_r, _r, false};
}

Radius Radius::zero() {
    return Radius{0, 0, false};
}

float Radius::x() const {
    return _x;
}

float Radius::y() const {
    return _y;
}

glm::vec2 Radius::toVector2() const {
    return glm::vec2{_x, _y};
}

Radius Radius::toScale(const glm::vec2 &viewportSize) const {
    if (!_isPixels) return *this;

    return Radius::ellipticalScale(_x / viewportSize.x, _y / viewportSize.y);
}

Radius Radius::toPixels(const glm::vec2 &viewportSize) const {
    if (_isPixels) return *this;

    return Radius::ellipticalPixel(_x * viewportSize.x, _y * viewportSize.y);
}

std::ostream &operator<< (std::ostream &os, const Radius &r) {
    os << "(<" << r._x << ", " << r._y << ">, isScale=" << (!r._isPixels ? "true" : "false") << ")";
    return os;
}

BorderRadius::BorderRadius(
    Radius topLeft,
    Radius topRight,
    Radius bottomLeft,
    Radius bottomRight
) : _topLeft{topLeft}, _topRight{topRight}, _bottomLeft{bottomLeft}, _bottomRight{bottomRight} {}

BorderRadius BorderRadius::all(Radius radius) {
    return BorderRadius(radius, radius, radius, radius);
}

BorderRadius BorderRadius::circularPixel(int radius) {
    return BorderRadius::all(Radius::circularPixel(radius));
}

BorderRadius BorderRadius::circularScale(float radius) {
    return BorderRadius::all(Radius::circularScale(radius));
}

BorderRadius BorderRadius::horizontal(Radius left, Radius right) {
    return BorderRadius(left, right, left, right);
}

BorderRadius BorderRadius::vertical(Radius top, Radius bottom) {
    return BorderRadius(top, top, bottom, bottom);
}

BorderRadius BorderRadius::zero() {
    return BorderRadius::all(Radius::zero());
}

Radius BorderRadius::topLeft() const {
    return _topLeft;
}

Radius BorderRadius::topRight() const {
    return _topRight;
}

Radius BorderRadius::bottomLeft() const {
    return _bottomLeft;
}

Radius BorderRadius::bottomRight() const {
    return _bottomRight;
}

BorderRadius BorderRadius::toScale(const glm::vec2 &viewportSize) const {
    return BorderRadius(
        _topLeft.toScale(viewportSize),
        _topRight.toScale(viewportSize),
        _bottomLeft.toScale(viewportSize),
        _bottomRight.toScale(viewportSize)
    );
}

BorderRadius BorderRadius::toPixels(const glm::vec2 &viewportSize) const {
    return BorderRadius(
        _topLeft.toPixels(viewportSize),
        _topRight.toPixels(viewportSize),
        _bottomLeft.toPixels(viewportSize),
        _bottomRight.toPixels(viewportSize)
    );
}

std::ostream &operator<< (std::ostream &os, const BorderRadius &br) {
    os << "(TL="     << br._topLeft    << ", TR="    << br._topRight
       << ", BL="    << br._bottomLeft << ", BR="    << br._bottomRight << ")";
    return os;
}