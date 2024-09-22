#include <algorithm>

#include "border_radius.hpp"

Radius::Radius(const Dim &x, const Dim &y) : _x{x}, _y{y} {}

Radius Radius::elliptical(const Dim2 &r) {
    Dim _x = Dim::max(Dim::zero(), r.x());
    Dim _y = Dim::max(Dim::zero(), r.y());
    return Radius{_x, _y};
}

Radius Radius::circular(const Dim &r) {
    auto _r = Dim::max(Dim::zero(), r);
    return Radius{_r, _r};
}

Radius Radius::zero() {
    return Radius{Dim::zero(), Dim::zero()};
}

Dim Radius::x() const {
    return _x;
}

Dim Radius::y() const {
    return _y;
}

glm::vec2 Radius::toScale(const glm::vec2 &viewportSize) const {
    return glm::vec2{_x.toScale(viewportSize.x), _y.toScale(viewportSize.y)};
}

glm::vec2 Radius::toPixels(const glm::vec2 &viewportSize) const {
    return glm::vec2{_x.toPixels(viewportSize.x), _y.toPixels(viewportSize.y)};
}

std::ostream &operator<< (std::ostream &os, const Radius &r) {
    os << "(" << r._x << ", " << r._y << ")";
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

BorderRadius BorderRadius::circular(const Dim &r) {
    return BorderRadius::all(Radius::circular(r));
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

std::ostream &operator<< (std::ostream &os, const BorderRadius &br) {
    os << "(TL="     << br._topLeft    << ", TR="    << br._topRight
       << ", BL="    << br._bottomLeft << ", BR="    << br._bottomRight << ")";
    return os;
}