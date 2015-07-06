#include <Dunjun/Math/Vector3.hpp>

#include <Dunjun/Math/Functions.hpp>

namespace Dunjun
{
const Vector3 Vector3::Zero = {0, 0};

bool operator==(const Vector3& a, const Vector3& b)
{
	for (usize i = 0; i < 3; i++)
	{
		if (a[i] != b[i])
			return false;
	}

	return true;
}

bool operator!=(const Vector3& a, const Vector3& b)
{
	return !operator==(a, b);
}

Vector3 operator-(const Vector3& a) { return {-a.x, -a.y, -a.z}; }

Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return {a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Vector3 operator*(const Vector3& a, f32 scalar)
{
	return {scalar * a.x, scalar * a.y, scalar * a.z};
}

Vector3 operator/(const Vector3& a, f32 scalar)
{
	return {a.x / scalar, a.y / scalar, a.z / scalar};
}

// Hadamard Product
Vector3 operator*(const Vector3& a, const Vector3& b)
{
	Vector3 result;
	for (usize i = 0; i < 3; i++)
		result[i] = a[i] * b[i];
	return result;
}

// Hadamard Product
Vector3 operator/(const Vector3& a, const Vector3& b)
{
	Vector3 result;
	for (usize i = 0; i < 3; i++)
		result[i] = a[i] / b[i];
	return result;
}

Vector3& operator+=(Vector3& a, const Vector3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;

	return a;
}

Vector3& operator-=(Vector3& a, const Vector3& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;

	return a;
}

Vector3& operator*=(Vector3& a, f32 scalar)
{
	a.x *= scalar;
	a.y *= scalar;
	a.z *= scalar;

	return a;
}

Vector3& operator/=(Vector3& a, f32 scalar)
{
	a.x /= scalar;
	a.y /= scalar;
	a.z /= scalar;

	return a;
}

f32 dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 cross(const Vector3& a, const Vector3& b)
{
	return {
	    a.y * b.z - b.y * a.z, // x
	    a.z * b.x - b.z * a.x, // y
	    a.x * b.y - b.x * a.y  // z
	};
}

f32 lengthSquared(const Vector3& a) { return dot(a, a); }

f32 length(const Vector3& a) { return Math::sqrt(lengthSquared(a)); }

Vector3 normalize(const Vector3& a) { return a * (1.0f / length(a)); }
} // namespace Dunjun
