#include <Dunjun/Math/Vector2.hpp>

#include <Dunjun/Math/Functions.hpp>

namespace Dunjun
{
bool operator==(const Vector2& a, const Vector2& b)
{
	for (usize i = 0; i < 2; i++)
	{
		if (a[i] != b[i])
			return false;
	}

	return true;
}

bool operator!=(const Vector2& a, const Vector2& b)
{
	return !operator==(a, b);
}

Vector2 operator-(const Vector2& a) { return {-a.x, -a.y}; }

Vector2 operator+(const Vector2& a, const Vector2& b)
{
	return {a.x + b.x, a.y + b.y};
}

Vector2 operator-(const Vector2& a, const Vector2& b)
{
	return {a.x - b.x, a.y - b.y};
}

Vector2 operator*(const Vector2& a, f32 scalar)
{
	return {scalar * a.x, scalar * a.y};
}

Vector2 operator/(const Vector2& a, f32 scalar)
{
	return {a.x / scalar, a.y / scalar};
}

// Hadamard Product
Vector2 operator*(const Vector2& a, const Vector2& b)
{
	Vector2 result;
	for (usize i = 0; i < 2; i++)
		result[i] = a[i] * b[i];
	return result;
}

// Hadamard Product
Vector2 operator/(const Vector2& a, const Vector2& b)
{
	Vector2 result;
	for (usize i = 0; i < 2; i++)
		result[i] = a[i] / b[i];
	return result;
}

Vector2& operator+=(Vector2& a, const Vector2& b)
{
	a.x += b.x;
	a.y += b.y;

	return a;
}

Vector2& operator-=(Vector2& a, const Vector2& b)
{
	a.x -= b.x;
	a.y -= b.y;

	return a;
}

Vector2& operator*=(Vector2& a, f32 scalar)
{
	a.x *= scalar;
	a.y *= scalar;

	return a;
}

Vector2& operator/=(Vector2& a, f32 scalar)
{
	a.x /= scalar;
	a.y /= scalar;

	return a;
}

f32 dot(const Vector2& a, const Vector2& b) { return a.x * b.x + a.y * b.y; }

f32 cross(const Vector2& a, const Vector2& b) { return a.x * b.y - b.x * a.y; }

f32 lengthSquared(const Vector2& a) { return dot(a, a); }

f32 length(const Vector2& a) { return Math::sqrt(lengthSquared(a)); }

Vector2 normalize(const Vector2& a) { return a * (1.0f / length(a)); }
} // namespace Dunjun
