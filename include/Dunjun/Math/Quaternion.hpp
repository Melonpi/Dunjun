#ifndef DUNJUN_MATH_QUATERNION_HPP
#define DUNJUN_MATH_QUATERNION_HPP

#include <Dunjun/Math/Types.hpp>
#include <Dunjun/Math/Angle.hpp>
#include <Dunjun/Math/Constants.hpp>

#include <ostream>

namespace Dunjun
{
Quaternion operator-(const Quaternion& a);
Quaternion operator+(const Quaternion& a, const Quaternion& b);
Quaternion operator-(const Quaternion& a, const Quaternion& b);

Quaternion operator*(const Quaternion& a, const Quaternion& b);
Quaternion operator*(const Quaternion& a, f32 s);
Quaternion operator/(const Quaternion& a, f32 s);

bool operator==(const Quaternion& a, const Quaternion& b);
bool operator!=(const Quaternion& a, const Quaternion& b);

inline Quaternion operator*(f32 s, const Quaternion& q) { return q * s; }

f32 dot(const Quaternion& a, const Quaternion& b);
Quaternion cross(const Quaternion& a, const Quaternion& b);

f32 lengthSquared(const Quaternion& q);
f32 length(const Quaternion& q);

Quaternion normalize(const Quaternion& q);

Quaternion conjugate(const Quaternion& q);
Quaternion inverse(const Quaternion& q);

// NOTE(bill): Rotate v by q
Vector3 operator*(const Quaternion& q, const Vector3& v);

inline std::ostream& operator<<(std::ostream& os, const Quaternion& q)
{
	os << "Quaternion{";
	for (usize i = 0; i < 4; i++)
	{
		os << q.data[i];
		if (i < 3)
			os << ", ";
	}
	os << "}";

	return os;
}

Radian angle(const Quaternion& q);
Vector3 axis(const Quaternion& q);
Quaternion angleAxis(const Radian& angle, const Vector3& axis);

Matrix4 quaternionToMatrix4(const Quaternion& q);
// NOTE(bill): Assumes matrix is only a rotational matrix and has no shear
//             applied
Quaternion matrix4ToQuaternion(const Matrix4& m);

struct EulerAngles
{
	Radian pitch;
	Radian yaw;
	Radian roll;
};

Radian roll(const Quaternion& q);
Radian pitch(const Quaternion& q);
Radian yaw(const Quaternion& q);

EulerAngles quaternionToEulerAngles(const Quaternion& q);
Quaternion eulerAnglesToQuaternion(const EulerAngles& e,
                                   const Vector3& xAxis = {1, 0, 0},
                                   const Vector3& yAxis = {0, 1, 0},
                                   const Vector3& zAxis = {0, 0, 1});
inline Quaternion eulerAnglesToQuaternion(const Radian& pitch,
                                          const Radian& yaw,
                                          const Radian& roll,
                                          const Vector3& xAxis = {1, 0, 0},
                                          const Vector3& yAxis = {0, 1, 0},
                                          const Vector3& zAxis = {0, 0, 1})
{
	return eulerAnglesToQuaternion({pitch, yaw, roll}, xAxis, yAxis, zAxis);
}

} // namespace Dunjun

#endif
