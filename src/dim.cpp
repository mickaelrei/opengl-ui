#include <algorithm>

#include "dim.hpp"

Dim::Dim(int pixels, float scale) : _pixels{pixels}, _scale{scale} {}

Dim Dim::zero() {
    return Dim{0, 0.0f};
}

Dim Dim::fromPixels(int pixels) {
    return Dim{pixels, 0.0f};
}

Dim Dim::fromScale(float scale) {
    return Dim{0, scale};
}

Dim Dim::max(const Dim &d0, const Dim &d1) {
    return Dim{
        std::max(d0.pixels(), d1.pixels()),
        std::max(d0.scale(), d1.scale())
    };
}

Dim Dim::min(const Dim &d0, const Dim &d1) {
    return Dim{
        std::min(d0.pixels(), d1.pixels()),
        std::min(d0.scale(), d1.scale())
    };
}

Dim Dim::clamp(const Dim &d, const Dim &min, const Dim &max) {
    return Dim{
        std::clamp(d.pixels(), min.pixels(), max.pixels()),
        std::clamp(d.scale(), min.scale(), max.scale()),
    };
}

Dim Dim::operator+ (const Dim &d) const {
    Dim tmp{*this};
    tmp += d;
    return tmp;
}
Dim &Dim:: operator+= (const Dim &d) {
    _pixels += d.pixels();
    _scale += d.scale();
    return *this;
}

Dim Dim::operator- () const {
    return Dim{-_pixels, -_scale};
}

Dim Dim::operator- (const Dim &d) const {
    Dim tmp{*this};
    tmp -= d;
    return tmp;
}

Dim &Dim::operator-= (const Dim &d) {
    *this += -d;
    return *this;
}

int Dim::pixels() const {
    return _pixels;
}

float Dim::scale() const {
    return _scale;
}

float Dim::toScale(float dimensionSize) const {
    return _scale + (float)_pixels / dimensionSize;
}

float Dim::toPixels(float dimensionSize) const {
    return _scale * dimensionSize + _pixels;
}

Dim2::Dim2(const Dim &x, const Dim &y) : _x{x}, _y{y} {}

Dim2::Dim2(
    int pixelsX , float scaleX,
    int pixelsY, float scaleY
) : _x{pixelsX, scaleX}, _y{pixelsY, scaleY} {}

Dim2 Dim2::zero() {
    return Dim2{Dim::zero(), Dim::zero()};
}

Dim2 Dim2::fromPixels(int x, int y) {
    return Dim2{Dim::fromPixels(x), Dim::fromPixels(y)};
}

Dim2 Dim2::fromScale(float x, float y) {
    return Dim2{Dim::fromScale(x), Dim::fromScale(y)};
}

Dim2 Dim2::max(const Dim2 &d0, const Dim2 &d1) {
    return Dim2{
        Dim::max(d0.x(), d1.x()),
        Dim::max(d0.y(), d1.y())
    };
}

Dim2 Dim2::min(const Dim2 &d0, const Dim2 &d1) {
    return Dim2{
        Dim::min(d0.x(), d1.x()),
        Dim::min(d0.y(), d1.y())
    };
}

Dim2 Dim2::clamp(const Dim2 &d, const Dim2 &min, const Dim2 &max) {
    return Dim2{
        Dim::clamp(d.x(), min.x(), max.x()),
        Dim::clamp(d.y(), min.y(), max.y()),
    };
}

Dim2 Dim2::operator+ (const Dim2 &d) const {
    Dim2 tmp{*this};
    tmp += d;
    return tmp;
}

Dim2 &Dim2::operator+= (const Dim2 &d) {
    _x += d.x();
    _y += d.y();
    return *this;
}

Dim2 Dim2::operator- () const {
    return Dim2{-_x, -_y};
}

Dim2 Dim2::operator- (const Dim2 &d) const {
    Dim2 tmp{*this};
    tmp -= d;
    return tmp;
}

Dim2 &Dim2::operator-= (const Dim2 &d) {
    *this += -d;
    return *this;
}

Dim Dim2::x() const {
    return _x;
}

Dim Dim2::y() const {
    return _y;
}

glm::vec2 Dim2::toScale(const glm::vec2 &windowSize) const {
    return glm::vec2{_x.toScale(windowSize.x), _y.toScale(windowSize.y)};
}

glm::vec2 Dim2::toPixels(const glm::vec2 &windowSize) const {
    return glm::vec2{_x.toPixels(windowSize.x), _y.toPixels(windowSize.y)};
}