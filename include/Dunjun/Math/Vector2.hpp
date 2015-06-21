#ifndef DUNJUN_MATH_VECTOR2_HPP
#define DUNJUN_MATH_VECTOR2_HPP

#include <Dunjun/System/Types.hpp>
#include <iostream>

namespace Dunjun
{
union Vector2
{
	struct
	{
		f32 x, y;
	};
	struct
	{
		f32 r, g;
	};
	struct
	{
		f32 s, t;
	};
	f32 data[2];
};

bool operator==(const Vector2& a, const Vector2& b);
bool operator!=(const Vector2& a, const Vector2& b);

Vector2 operator-(const Vector2& a);

Vector2 operator+(const Vector2& a, const Vector2& b);
Vector2 operator-(const Vector2& a, const Vector2& b);

Vector2 operator*(const Vector2& a, f32 scalar);

Vector2 operator/(const Vector2& a, f32 scalar);

// Hadamard Product
Vector2 operator*(const Vector2& a, const Vector2& b);
// Hadamard Product
Vector2 operator/(const Vector2& a, const Vector2& b);

Vector2& operator+=(Vector2& a, const Vector2& b);
Vector2& operator-=(Vector2& a, const Vector2& b);
Vector2& operator*=(Vector2& a, f32 scalar);

Vector2& operator/=(Vector2& a, f32 scalar);

inline Vector2 operator*(f32 scalar, const Vector2& vector)
{
	return vector * scalar;
}

f32 dot(const Vector2& a, const Vector2& b);
f32 cross(const Vector2& a, const Vector2& b);

f32 lengthSquared(const Vector2& a);
f32 length(const Vector2& a);

Vector2 normalize(const Vector2& a);

inline std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
	return os << "Vector2{" << v.x << ", " << v.y << "}";
}
} // namespace Dunjun

#endif
