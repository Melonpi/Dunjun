#ifndef DUNJUN_MATH_VECTOR3_HPP
#define DUNJUN_MATH_VECTOR3_HPP

#include <Dunjun/Math/Types.hpp>
#include <ostream>

namespace Dunjun
{
bool operator==(const Vector3& a, const Vector3& b);
bool operator!=(const Vector3& a, const Vector3& b);

Vector3 operator-(const Vector3& a);

Vector3 operator+(const Vector3& a, const Vector3& b);
Vector3 operator-(const Vector3& a, const Vector3& b);
Vector3 operator*(const Vector3& a, f32 scalar);
Vector3 operator/(const Vector3& a, f32 scalar);

// Hadamard Product
Vector3 operator*(const Vector3& a, const Vector3& b);
// Hadamard Product
Vector3 operator/(const Vector3& a, const Vector3& b);
Vector3& operator+=(Vector3& a, const Vector3& b);
Vector3& operator-=(Vector3& a, const Vector3& b);

Vector3& operator*=(Vector3& a, f32 scalar);

Vector3& operator/=(Vector3& a, f32 scalar);

inline Vector3 operator*(f32 scalar, const Vector3& vector)
{
	return vector * scalar;
}

f32 dot(const Vector3& a, const Vector3& b);
Vector3 cross(const Vector3& a, const Vector3& b);

f32 lengthSquared(const Vector3& a);
f32 length(const Vector3& a);

Vector3 normalize(const Vector3& a);

inline std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	return os << "Vector3{" << v.x << ", " << v.y << ", " << v.z << "}";
}

} // namespace Dunjun

#endif
