#include <Dunjun/Math/Quaternion.hpp>
#include <Dunjun/Math/Functions.hpp>

namespace Dunjun
{
const Quaternion Quaternion::Identity = {0, 0, 0, 1};

Quaternion operator-(const Quaternion& a)
{
	return Quaternion{-a.x, -a.y, -a.z, -a.w};
}

Quaternion operator+(const Quaternion& a, const Quaternion& b)
{
	Quaternion c;

	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	c.w = a.w + b.w;

	return c;
}

Quaternion operator-(const Quaternion& a, const Quaternion& b)
{
	Quaternion c;

	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;

	return c;
}

Quaternion operator*(const Quaternion& a, const Quaternion& b)
{
	Quaternion c{};

	// q = (v, s) -> where v = Vector3, s = Scalar (f32)
	// (v1, s1)(v2, s2) = (s1v2 + s2v1 + v1 x v2, s1s2 - v1 . v2)
	// . -> dot product
	// x -> cross product

	c.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
	c.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
	c.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
	c.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;

	return c;
}

Quaternion operator*(const Quaternion& a, f32 s)
{
	Quaternion c = a;

	c.x *= s;
	c.y *= s;
	c.z *= s;
	c.w *= s;

	return c;
}

Quaternion operator/(const Quaternion& a, f32 s)
{
	Quaternion c = a;

	c.x /= s;
	c.y /= s;
	c.z /= s;
	c.w /= s;

	return c;
}

bool operator==(const Quaternion& a, const Quaternion& b)
{
	for (usize i = 0; i < 4; i++)
	{
		if (a.data[i] != b.data[i])
			return false;
	}
	return true;
}

bool operator!=(const Quaternion& a, const Quaternion& b)
{
	return !operator==(a, b);
}

f32 lengthSquared(const Quaternion& q) { return dot(q, q); }

f32 length(const Quaternion& q) { return Math::sqrt(lengthSquared(q)); }

f32 dot(const Quaternion& a, const Quaternion& b)
{
	return dot(a.xyz, b.xyz) + a.w * b.w;
}

Quaternion cross(const Quaternion& a, const Quaternion& b)
{
	return Quaternion{a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
	                  a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
	                  a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x,
	                  a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z};
}

Quaternion normalize(const Quaternion& q) { return q * (1.0f / length(q)); }

Quaternion conjugate(const Quaternion& q)
{
	return Quaternion{-q.x, -q.y, -q.z, q.w};
}

Quaternion inverse(const Quaternion& q) { return conjugate(q) / dot(q, q); }

Vector3 operator*(const Quaternion& q, const Vector3& v)
{
	// return q * Quaternion(v, 0) * conjugate(q); // More Expensive
	const Vector3 t = 2.0f * cross(q.xyz, v);
	return (v + q.w * t + cross(q.xyz, t));
}

Radian angle(const Quaternion& q) { return 2.0f * Math::acos(q.w); }

Vector3 axis(const Quaternion& q)
{
	// 1 - cos(theta/2)*cos(theta/2) = sin(theta/2)*sin(theta/2)
	const f32 s2 = 1.0f - q.w * q.w;

	if (s2 <= 0.0f)
		return Vector3{0, 0, 1};

	const f32 invs2 = 1.0f / Math::sqrt(s2);

	return q.xyz * invs2;
}

Quaternion angleAxis(const Radian& angle, const Vector3& axis)
{
	Quaternion q;

	const Vector3 a = normalize(axis);

	const f32 s = Math::sin(0.5f * angle);

	q.xyz = a * s;
	q.w   = Math::cos(0.5f * angle);

	return q;
}

Matrix4 quaternionToMatrix4(const Quaternion& q)
{
	Matrix4 mat        = Matrix4::Identity;
	const Quaternion a = normalize(q);

	const f32 xx = a.x * a.x;
	const f32 yy = a.y * a.y;
	const f32 zz = a.z * a.z;
	const f32 xy = a.x * a.y;
	const f32 xz = a.x * a.z;
	const f32 yz = a.y * a.z;
	const f32 wx = a.w * a.x;
	const f32 wy = a.w * a.y;
	const f32 wz = a.w * a.z;

	mat[0][0] = 1.0f - 2.0f * (yy + zz);
	mat[0][1] = 2.0f * (xy + wz);
	mat[0][2] = 2.0f * (xz - wy);

	mat[1][0] = 2.0f * (xy - wz);
	mat[1][1] = 1.0f - 2.0f * (xx + zz);
	mat[1][2] = 2.0f * (yz + wx);

	mat[2][0] = 2.0f * (xz + wy);
	mat[2][1] = 2.0f * (yz - wx);
	mat[2][2] = 1.0f - 2.0f * (xx + yy);

	return mat;
}

// NOTE(bill): Assumes matrix is only a rotational matrix and has no shear
//             applied
Quaternion matrix4ToQuaternion(const Matrix4& m)
{
	const f32 fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
	const f32 fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
	const f32 fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
	const f32 fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

	s32 biggestIndex = 0;
	f32 fourBiggestSquaredMinus1{fourWSquaredMinus1};
	if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex             = 1;
	}
	if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex             = 2;
	}
	if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
	{
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex             = 3;
	}

	const f32 biggestVal = Math::sqrt(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
	const f32 mult       = 0.25f / biggestVal;

	Quaternion q = Quaternion{0, 0, 0, 1};

	switch (biggestIndex)
	{
	case 0:
	{
		q.w = biggestVal;
		q.x = (m[1][2] - m[2][1]) * mult;
		q.y = (m[2][0] - m[0][2]) * mult;
		q.z = (m[0][1] - m[1][0]) * mult;
	}
	break;
	case 1:
	{
		q.w = (m[1][2] - m[2][1]) * mult;
		q.x = biggestVal;
		q.y = (m[0][1] + m[1][0]) * mult;
		q.z = (m[2][0] + m[0][2]) * mult;
	}
	break;
	case 2:
	{
		q.w = (m[2][0] - m[0][2]) * mult;
		q.x = (m[0][1] + m[1][0]) * mult;
		q.y = biggestVal;
		q.z = (m[1][2] + m[2][1]) * mult;
	}
	break;
	case 3:
	{
		q.w = (m[0][1] - m[1][0]) * mult;
		q.x = (m[2][0] + m[0][2]) * mult;
		q.y = (m[1][2] + m[2][1]) * mult;
		q.z = biggestVal;
	}
	break;
	default: // Should never actually get here. Just for sanities sake.
	{
		assert(false && "How did you get here?!");
	}
	break;
	}

	return q;
}

Radian roll(const Quaternion& q)
{
	return Math::atan2(2.0f * q.x * q.y + q.z * q.w,
	                   q.x * q.x + q.w * q.w - q.y * q.y - q.z * q.z);
}

Radian pitch(const Quaternion& q)
{
	return Math::atan2(2.0f * q.y * q.z + q.w * q.x,
	                   q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
}

Radian yaw(const Quaternion& q)
{
	return Math::asin(-2.0f * (q.x * q.z - q.w * q.y));
}

EulerAngles quaternionToEulerAngles(const Quaternion& q)
{
	return {pitch(q), yaw(q), roll(q)};
}

Quaternion eulerAnglesToQuaternion(const EulerAngles& e,
                                   const Vector3& xAxis,
                                   const Vector3& yAxis,
                                   const Vector3& zAxis)
{
	const Quaternion p = angleAxis(e.pitch, xAxis);
	const Quaternion y = angleAxis(e.pitch, yAxis);
	const Quaternion r = angleAxis(e.pitch, zAxis);

	return y * p * r;
}

} // namespace Dunjun
