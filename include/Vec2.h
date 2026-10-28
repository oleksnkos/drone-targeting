#pragma once

struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    double length() const;
};

Vec2 operator+(const Vec2& a, const Vec2& b);
Vec2 operator-(const Vec2& a, const Vec2& b);
Vec2 operator*(const Vec2& v, double k);
Vec2 operator*(double k, const Vec2& v);

double distance(const Vec2& a, const Vec2& b);