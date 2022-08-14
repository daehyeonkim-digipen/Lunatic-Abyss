#pragma once
#include <algorithm>
#include <random>

namespace math {
    [[nodiscard]] constexpr double abs(double d) noexcept { return (d < 0.0) ? -d : d; }
    [[nodiscard]] constexpr bool   is_equal(double d1, double d2) noexcept {
        return abs(d1 - d2) <= std::numeric_limits<double>::epsilon() * abs(d1 + d2);
    }
    [[nodiscard]] constexpr bool   is_equal(int i1, int i2) noexcept {
        return i1 == i2;
    }

    struct [[nodiscard]] vec2 {
        double x{ 0.0 };
        double y{ 0.0 };


        constexpr vec2() noexcept = default;
        constexpr vec2(double x_value, double y_value) noexcept : x{ x_value }, y{ y_value } {};
        constexpr vec2(int x_value, int y_value) noexcept : x{ static_cast<double>(x_value) }, y{ static_cast<double>(y_value) } {};

        [[nodiscard]] constexpr bool operator==(const vec2& v) const noexcept;
        [[nodiscard]] constexpr bool operator!=(const vec2& v) const noexcept;

        constexpr vec2  operator+(const vec2& v) const noexcept;
        constexpr vec2& operator+=(const vec2& v) noexcept;

        constexpr vec2  operator-(const vec2& v) const noexcept;
        constexpr vec2& operator-=(const vec2& v) noexcept;

        constexpr vec2 operator-() const noexcept;

        constexpr vec2& operator+=(double value) noexcept;
        constexpr vec2& operator-=(double value) noexcept;

        constexpr vec2  operator*(double scale) const noexcept;
        constexpr vec2  operator/(double divisor) const noexcept;
        constexpr vec2& operator*=(double scale) noexcept;
        constexpr vec2& operator/=(double divisor) noexcept;

        constexpr vec2 operator*(vec2 scale) noexcept;
        constexpr vec2& operator*=(vec2 scale) noexcept;

        constexpr bool operator<(vec2 other) noexcept;
        constexpr bool operator>(vec2 other) noexcept;
        constexpr bool operator<=(vec2 other) noexcept;
        constexpr bool operator>=(vec2 other) noexcept;

        vec2 Normalize() const noexcept;
        constexpr double Cross(const vec2& v) const noexcept;
    	double Distance(const vec2& v) const noexcept;
        double Length() const noexcept;

    };

    constexpr vec2 operator*(double scale, const vec2& v) noexcept;

    constexpr vec2 LinearDivision(const vec2& v1, const vec2& v2) noexcept;

    struct [[nodiscard]] rect2 {
        vec2 point1{ 0.0, 0.0 };
        vec2 point2{ 0.0, 0.0 };
        constexpr vec2 Center() const noexcept { return { (Right() + Left()) / 2., (Top() + Bottom()) / 2. }; }
        constexpr vec2 Size() const noexcept { return { Right() - Left(), Top() - Bottom() }; }
        constexpr double Left() const noexcept { return std::min(point1.x, point2.x); }     // if the rect was flipped, then bottomLeft.x > topRight.x
        constexpr double Right() const noexcept { return std::max(point1.x, point2.x); }    // if the rect was flipped, then bottomLeft.x > topRight.x
        constexpr double Top() const noexcept { return std::max(point1.y, point2.y); }   // if the rect uses top left as {0, 0} or was flipped in the y direction
        constexpr double Bottom() const noexcept { return std::min(point1.y, point2.y); }   // if the rect uses top left as {0, 0} or was flipped in the y direction
    };

    class TransformMatrix {
    public:
        TransformMatrix();

        const double* operator[](int index) const { return matrix[index]; }
        TransformMatrix operator * (TransformMatrix rhs) const;
        TransformMatrix& operator *= (TransformMatrix rhs);
        vec2 operator * (vec2 rhs) const;
        void Reset(); 
    protected:
        double matrix[3][3];
    };

    class TranslateMatrix : public TransformMatrix {
    public:
        TranslateMatrix(vec2 translate);
    };

    class RotateMatrix : public TransformMatrix {
    public:
        RotateMatrix(double radians);
    };

    class ScaleMatrix : public TransformMatrix {
    public:
        ScaleMatrix(vec2 scale);
    };
}

#include "Math.inl"