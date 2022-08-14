#pragma once   
#include <cmath>

namespace math
{
    constexpr bool vec2::operator==(const vec2& v) const noexcept { return is_equal(x, v.x) && is_equal(y, v.y); }
    constexpr bool vec2::operator!=(const vec2& v) const noexcept { return !operator==(v); }

    constexpr vec2 vec2::operator+(const vec2& v) const noexcept { return { x + v.x, y + v.y }; }
    constexpr vec2& vec2::operator+=(const vec2& v) noexcept
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    constexpr vec2 vec2::operator-(const vec2& v) const noexcept { return { x - v.x, y - v.y }; }
    constexpr vec2& vec2::operator-=(const vec2& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    constexpr vec2 vec2::operator-() const noexcept { return { -x, -y }; }

    constexpr vec2& vec2::operator+=(double value) noexcept 
    {
        x += value;
        y += value;
        return *this;
    }

    constexpr vec2& vec2::operator-=(double value) noexcept
    {
        x -= value;
        y -= value;
        return *this;
    }

    constexpr vec2 vec2::operator*(double scale) const noexcept { return { x * scale, y * scale }; }
    constexpr vec2 vec2::operator/(double divisor) const noexcept { return { x / divisor, y / divisor }; }

    constexpr vec2& vec2::operator*=(double scale) noexcept
    {
        x *= scale, y *= scale;
        return *this;
    }
    constexpr vec2& vec2::operator/=(double divisor) noexcept
    {
        x /= divisor, y /= divisor;
        return *this;
    }
    constexpr vec2 operator*(double scale, const vec2& v) noexcept { return v * scale; }

    constexpr vec2 LinearDivision(const vec2& v1, const vec2& v2) noexcept { return { v1.x / v2.x, v1.y / v2.y }; };

    constexpr vec2 vec2::operator*(vec2 scale) noexcept { return { x * scale.x, y * scale.y }; }
    constexpr vec2& vec2::operator*=(vec2 scale) noexcept
    {
        x *= scale.x, y *= scale.y;
        return *this;
    }

    constexpr bool vec2::operator<(vec2 other) noexcept { return x < other.x&& y < other.y; }
    constexpr bool vec2::operator>(vec2 other) noexcept { return x > other.x&& y > other.y; }
    constexpr bool vec2::operator<=(vec2 other) noexcept { return x <= other.x&& y <= other.y; }
    constexpr bool vec2::operator>=(vec2 other) noexcept { return x >= other.x && y >= other.y; }

    inline vec2 vec2::Normalize() const noexcept {
        double length = std::sqrt(x * x + y * y);
        return { x / length, y / length };
    }
    constexpr double vec2::Cross(const vec2& v) const noexcept { return x * v.y - y * v.x; }

    inline double vec2::Distance(const vec2& v) const noexcept
    {
	    return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
    }

    inline double vec2::Length() const noexcept {
        return Distance({ 0, 0 });
    }

    inline TransformMatrix::TransformMatrix() {
        Reset();
    }

    inline void TransformMatrix::Reset() {
        matrix[0][0] = 1.0;     matrix[0][1] = 0.0;     matrix[0][2] = 0.0;
        matrix[1][0] = 0.0;     matrix[1][1] = 1.0;     matrix[1][2] = 0.0;
        matrix[2][0] = 0.0;     matrix[2][1] = 0.0;     matrix[2][2] = 1.0;
    }

    inline TranslateMatrix::TranslateMatrix(vec2 translate) {
        matrix[0][0] = 1;        matrix[0][1] = 0;        matrix[0][2] = translate.x;
        matrix[1][0] = 0;        matrix[1][1] = 1;        matrix[1][2] = translate.y;
        matrix[2][0] = 0;        matrix[2][1] = 0;        matrix[2][2] = 1;
    }

    inline RotateMatrix::RotateMatrix(double radians) {
        matrix[0][0] = std::cos(radians);    matrix[0][1] = -std::sin(radians);     matrix[0][2] = 0;
        matrix[1][0] = std::sin(radians);    matrix[1][1] = std::cos(radians);      matrix[1][2] = 0;
        matrix[2][0] = 0;                    matrix[2][1] = 0;                      matrix[2][2] = 1;
    }

    inline ScaleMatrix::ScaleMatrix(vec2 scale) {
        matrix[0][0] = scale.x;             matrix[0][1] = 0;                       matrix[0][2] = 0;
        matrix[1][0] = 0;                   matrix[1][1] = scale.y;                 matrix[1][2] = 0;
        matrix[2][0] = 0;                   matrix[2][1] = 0;                       matrix[2][2] = 1;
    }

    inline TransformMatrix TransformMatrix::operator * (TransformMatrix rhs) const {
        TransformMatrix result;

        result.matrix[0][0] = matrix[0][0] * rhs[0][0] + matrix[0][1] * rhs[1][0] + matrix[0][2] * rhs[2][0];
        result.matrix[0][1] = matrix[0][0] * rhs[0][1] + matrix[0][1] * rhs[1][1] + matrix[0][2] * rhs[2][1];
        result.matrix[0][2] = matrix[0][0] * rhs[0][2] + matrix[0][1] * rhs[1][2] + matrix[0][2] * rhs[2][2];
        result.matrix[1][0] = matrix[1][0] * rhs[0][0] + matrix[1][1] * rhs[1][0] + matrix[1][2] * rhs[2][0];
        result.matrix[1][1] = matrix[1][0] * rhs[0][1] + matrix[1][1] * rhs[1][1] + matrix[1][2] * rhs[2][1];
        result.matrix[1][2] = matrix[1][0] * rhs[0][2] + matrix[1][1] * rhs[1][2] + matrix[1][2] * rhs[2][2];
        result.matrix[2][0] = matrix[2][0] * rhs[0][0] + matrix[2][1] * rhs[1][0] + matrix[2][2] * rhs[2][0];
        result.matrix[2][1] = matrix[2][0] * rhs[0][1] + matrix[2][1] * rhs[1][1] + matrix[2][2] * rhs[2][1];
        result.matrix[2][2] = matrix[2][0] * rhs[0][2] + matrix[2][1] * rhs[1][2] + matrix[2][2] * rhs[2][2];

        return result;
    }

    inline vec2 TransformMatrix::operator * (vec2 rhs) const {
        vec2 result;
        result.x = matrix[0][0] * rhs.x + matrix[0][1] * rhs.y + matrix[0][2];
        result.y = matrix[1][0] * rhs.x + matrix[1][1] * rhs.y + matrix[1][2];
        return result;
    }

    inline TransformMatrix& TransformMatrix::operator *= (TransformMatrix rhs) {
        (*this) = (*this) * rhs;
        return (*this);
    }
}