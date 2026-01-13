#pragma once
#include <cmath>

namespace kern {

struct Vector2 {
    float x, y;

    constexpr Vector2() noexcept : x(0), y(0) {}
    constexpr Vector2(float x, float y) noexcept : x(x), y(y) {}

    static constexpr Vector2 zero() noexcept { return {0, 0}; }
    static constexpr Vector2 one()  noexcept { return {1, 1}; }

    // Define trivial constexpr operators inline
    constexpr Vector2 operator+(const Vector2& o) const noexcept { return {x + o.x, y + o.y}; }
    constexpr Vector2 operator-(const Vector2& o) const noexcept { return {x - o.x, y - o.y}; }
    constexpr Vector2 operator*(float s) const noexcept { return {x * s, y * s}; }
    constexpr Vector2 operator/(float s) const noexcept { return {x / s, y / s}; }
    constexpr Vector2 operator/(const Vector2& o) const noexcept { return {x / o.x, y / o.y}; }
    constexpr bool operator==(const Vector2& o) const noexcept { return x == o.x && y == o.y; }

    // Fixed compound assignment operators
    constexpr Vector2& operator+=(const Vector2& o) noexcept { x += o.x; y += o.y; return *this; }
    constexpr Vector2& operator-=(const Vector2& o) noexcept { x -= o.x; y -= o.y; return *this; }
    constexpr Vector2& operator*=(float s) noexcept { x *= s; y *= s; return *this; }
    constexpr Vector2& operator/=(float s) noexcept { x /= s; y /= s; return *this; }

    float length() const noexcept;
    float lengthSq() const noexcept;

    Vector2 normalized() const noexcept;
    void normalize() noexcept;

    static float dot(const Vector2& a, const Vector2& b) noexcept;

    static float distance(const Vector2& a, const Vector2& b) noexcept;
    static float distanceSq(const Vector2& a, const Vector2& b) noexcept;

    Vector2 perp() const noexcept;          // (-y, x)
    Vector2 abs() const noexcept;

    Vector2 floor() const noexcept;
    Vector2 ceil() const noexcept;
    Vector2 round() const noexcept;

    bool isZero(float eps = 1e-6f) const noexcept;

};

struct Vector3 {
    float x, y, z;

    constexpr Vector3() noexcept : x(0), y(0), z(0) {}
    constexpr Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

    static constexpr Vector3 zero() noexcept { return {0, 0, 0}; }
    static constexpr Vector3 one()  noexcept { return {1, 1, 1}; }

    // Define trivial constexpr operators inline
    constexpr Vector3 operator+(const Vector3& o) const noexcept { return {x + o.x, y + o.y, z + o.z}; }
    constexpr Vector3 operator-(const Vector3& o) const noexcept { return {x - o.x, y - o.y, z - o.z}; }
    constexpr Vector3 operator*(float s) const noexcept { return {x * s, y * s, z * s}; }
    constexpr Vector3 operator/(float s) const noexcept { return {x / s, y / s, z / s}; }
    constexpr bool operator==(const Vector3& o) const noexcept { return x == o.x && y == o.y && z == o.z; }

    // Fixed compound assignment operators
    constexpr Vector3& operator+=(const Vector3& o) noexcept { x += o.x; y += o.y; z += o.z; return *this; }
    constexpr Vector3& operator-=(const Vector3& o) noexcept { x -= o.x; y -= o.y; z -= o.z; return *this; }
    constexpr Vector3& operator*=(float s) noexcept { x *= s; y *= s; z *= s; return *this; }
    constexpr Vector3& operator/=(float s) noexcept { x /= s; y /= s; z /= s; return *this; }

    float length() const noexcept;
    float lengthSq() const noexcept;

    Vector3 normalized() const noexcept;
    void normalize() noexcept;

    static float dot(const Vector3& a, const Vector3& b) noexcept;

    static float distance(const Vector3& a, const Vector3& b) noexcept;
    static float distanceSq(const Vector3& a, const Vector3& b) noexcept;

    Vector3 perp() const noexcept;          // (-y, x)
    Vector3 abs() const noexcept;

    Vector3 floor() const noexcept;
    Vector3 ceil() const noexcept;
    Vector3 round() const noexcept;

    bool isZero(float eps = 1e-6f) const noexcept;

};

inline Vector2 screenToNDC(Vector2 screen, Vector2 size) {
    return Vector2(
        (screen.x / size.x) * 2.0f - 1.0f,
        -(screen.y / size.y) * 2.0f + 1.0f
    );
}

} // namespace kern
