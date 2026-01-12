#include "vectors.h"

#include <cmath>

namespace kern
{
    // VECTOR 2

    float Vector2::length() const noexcept { return std::sqrtf(x * x + y * y); }
    float Vector2::lengthSq() const noexcept { return x * x + y * y; }

    Vector2 Vector2::normalized() const noexcept
    {
        const float l2 = lengthSq();
        if (l2 < 1e-12f)
            return { 0.0f, 0.0f };
        return *this / std::sqrtf(l2);
    }

    void Vector2::normalize() noexcept
    {
        if (lengthSq() > 1e-12f)
        {
            *this /= length();
        }
    }

    float Vector2::dot(const Vector2& a, const Vector2& b) noexcept { return a.x * b.x + a.y * b.y; }
    float Vector2::distance(const Vector2& a, const Vector2& b) noexcept { return (a - b).length(); }
    float Vector2::distanceSq(const Vector2& a, const Vector2& b) noexcept { return (a - b).lengthSq(); }
    Vector2 Vector2::perp() const noexcept { return { -y, x }; }
    Vector2 Vector2::abs() const noexcept { return { std::abs(x), std::abs(y) }; }
    Vector2 Vector2::floor() const noexcept { return { std::floor(x), std::floor(y) }; }
    Vector2 Vector2::ceil() const noexcept { return { std::ceil(x), std::ceil(y) }; }
    Vector2 Vector2::round() const noexcept { return { std::round(x), std::round(y) }; }
    bool Vector2::isZero(float eps) const noexcept { return std::abs(x) < eps && std::abs(y) < eps; }

    // VECTOR 3

    float Vector3::length() const noexcept { return std::sqrtf(x * x + y * y + z * z); }
    float Vector3::lengthSq() const noexcept { return x * x + y * y + z * z; }

    Vector3 Vector3::normalized() const noexcept
    {
        const float l2 = lengthSq();
        if (l2 < 1e-12f)
            return { 0.0f, 0.0f, 0.0f };
        return *this / std::sqrtf(l2);
    }

    void Vector3::normalize() noexcept
    {
        if (lengthSq() > 1e-12f)
        {
            *this /= length();
        }
    }

    float Vector3::dot(const Vector3& a, const Vector3& b) noexcept { return a.x * b.x + a.y * b.y + a.z * b.z; }
    float Vector3::distance(const Vector3& a, const Vector3& b) noexcept { return (a - b).length(); }
    float Vector3::distanceSq(const Vector3& a, const Vector3& b) noexcept { return (a - b).lengthSq(); }
    Vector3 Vector3::perp() const noexcept { return { -y, x, z }; }
    Vector3 Vector3::abs() const noexcept { return { std::abs(x), std::abs(y), std::abs(z) }; }
    Vector3 Vector3::floor() const noexcept { return { std::floor(x), std::floor(y), std::floor(z) }; }
    Vector3 Vector3::ceil() const noexcept { return { std::ceil(x), std::ceil(y), std::ceil(z) }; }
    Vector3 Vector3::round() const noexcept { return { std::round(x), std::round(y), std::round(z) }; }
    bool Vector3::isZero(float eps) const noexcept { return std::abs(x) < eps && std::abs(y) < eps && std::abs(z) < eps; }
}