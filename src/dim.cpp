#include "dim.hpp"

Dim::Dim(int pixels, float scale) : _pixels{pixels}, _scale{scale} {}

Dim Dim::fromPixels(int pixels) {
    return Dim{pixels, 0.0f};
}

Dim Dim::fromScale(float scale) {
    return Dim{0, scale};
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

Dim2::Dim2(const Dim &x, const Dim &y) : _x{x}, _y{y} {}

Dim2::Dim2(
    int pixelsX , float scaleX,
    int pixelsY, float scaleY
) : _x{pixelsX, scaleX}, _y{pixelsY, scaleY} {}

Dim2 Dim2::fromPixels(int x, int y) {
    return Dim2{Dim::fromPixels(x), Dim::fromPixels(y)};
}

Dim2 Dim2::fromScale(float x, float y) {
    return Dim2{Dim::fromScale(x), Dim::fromScale(y)};
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