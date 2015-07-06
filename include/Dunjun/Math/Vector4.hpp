#ifndef DUNJUN_MATH_VECTOR4_HPP
#define DUNJUN_MATH_VECTOR4_HPP

#include <Dunjun/Math/Types.hpp>
#include <ostream>

namespace Dunjun
{
bool operator==(const Vector4& a, const Vector4& b);
bool operator!=(const Vector4& a, const Vector4& b);

Vector4 operator-(const Vector4& a);

Vector4 operator+(const Vector4& a, const Vector4& b);
Vector4 operator-(const Vector4& a, const Vector4& b);

Vector4 operator*(const Vector4& a, f32 scalar);

// Hadamard Product
Vector4 operator*(const Vector4& a, const Vector4& b);
// Hadamard Product
Vector4 operator/(const Vector4& a, const Vector4& b);
Vector4 operator/(const Vector4& a, f32 scalar);

Vector4& operator+=(Vector4& a, const Vector4& b);
Vector4& operator-=(Vector4& a, const Vector4& b);
Vector4& operator*=(Vector4& a, f32 scalar);
Vector4& operator/=(Vector4& a, f32 scalar);

inline Vector4 operator*(f32 scalar, const Vector4& vector)
{
	return vector * scalar;
}

f32 dot(const Vector4& a, const Vector4& b);

f32 lengthSquared(const Vector4& a);
f32 length(const Vector4& a);
Vector4 normalize(const Vector4& a);

inline std::ostream& operator<<(std::ostream& os, const Vector4& v)
{
	return os << "Vector4{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w
	          << "}";
}

} // namespace Dunjun

#endif
