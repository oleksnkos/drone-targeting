#include "Vec2.h"

#include <cmath>

double Vec2::length() const {
    return std::sqrt(x * x + y * y);
}

Vec2 operator+(const Vec2& a, const Vec2& b) {
    return {a.x + b.x, a.y + b.y};
}

Vec2 operator-(const Vec2& a, const Vec2& b) {
    return {a.x - b.x, a.y - b.y};
}

Vec2 operator*(const Vec2& v, double k) {
    return {v.x * k, v.y * k};
}

// implement second time, so that * is commutative
Vec2 operator*(double k, const Vec2& v) {
    return v * k;
}

double distance(const Vec2& a, const Vec2& b) {
    return (b - a).length();
}