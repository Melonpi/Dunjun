#include <Dunjun/Math/Vector4.hpp>

#include <Dunjun/Math/Functions.hpp>

namespace Dunjun
{
bool operator==(const Vector4& a, const Vector4& b)
{
	for (usize i{0}; i < 4; i++)
	{
		if (a.data[i] != b.data[i])
			return false;
	}

	return true;
}

bool operator!=(const Vector4& a, const Vector4& b)
{
	return !operator==(a, b);
}

Vector4 operator-(const Vector4& a) { return {-a.x, -a.y, -a.z, -a.w}; }

Vector4 operator+(const Vector4& a, const Vector4& b)
{
	return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

Vector4 operator-(const Vector4& a, const Vector4& b)
{
	return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

Vector4 operator*(const Vector4& a, f32 scalar)
{
	return {scalar * a.x, scalar * a.y, scalar * a.z, scalar * a.w};
}

// Hadamard Product
Vector4 operator*(const Vector4& a, const Vector4& b)
{
	Vector4 result;
	for (usize i{0}; i < 4; i++)
		result.data[i] = a.data[i] * b.data[i];
	return result;
}

// Hadamard Product
Vector4 operator/(const Vector4& a, const Vector4& b)
{
	Vector4 result;
	for (usize i{0}; i < 4; i++)
		result.data[i] = a.data[i] / b.data[i];
	return result;
}

Vector4 operator/(const Vector4& a, f32 scalar)
{
	return {a.x / scalar, a.y / scalar, a.z / scalar, a.w / scalar};
}

Vector4& operator+=(Vector4& a, const Vector4& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;

	return a;
}

Vector4& operator-=(Vector4& a, const Vector4& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.w -= b.w;

	return a;
}

Vector4& operator*=(Vector4& a, f32 scalar)
{
	a.x *= scalar;
	a.y *= scalar;
	a.z *= scalar;
	a.w *= scalar;

	return a;
}

Vector4& operator/=(Vector4& a, f32 scalar)
{
	a.x /= scalar;
	a.y /= scalar;
	a.z /= scalar;
	a.w /= scalar;

	return a;
}

f32 dot(const Vector4& a, const Vector4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

f32 lengthSquared(const Vector4& a) { return dot(a, a); }

f32 length(const Vector4& a) { return Math::sqrt(lengthSquared(a)); }

Vector4 normalize(const Vector4& a) { return a * (1.0f / length(a)); }

} // namespace Dunjun
