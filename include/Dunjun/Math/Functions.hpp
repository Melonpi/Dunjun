#ifndef DUNJUN_MATH_FUNCTIONS_HPP
#define DUNJUN_MATH_FUNCTIONS_HPP

////////////////////////////////
// Common
////////////////////////////////

#include <Dunjun/Common.hpp>
#include <Dunjun/Math/Angle.hpp>
#include <Dunjun/Math/Vector3.hpp>
#include <Dunjun/Math/Matrix4.hpp>
#include <Dunjun/Math/Quaternion.hpp>

#include <algorithm>
#include <cassert>

namespace Dunjun
{
namespace Math
{
// Trigonometric
f32 sin(const Radian& theta);
f32 cos(const Radian& theta);
f32 tan(const Radian& theta);

Radian asin(f32 a);
Radian acos(f32 a);
Radian atan(f32 a);
Radian atan2(f32 y, f32 x);

// Hyperbolic
f32 sinh(f32 x);
f32 cosh(f32 x);
f32 tanh(f32 x);

f32 asinh(f32 x);
f32 acosh(f32 x);
f32 atanh(f32 x);

// Power
f32 pow(f32 x, f32 y);
f32 sqrt(f32 x);
f32 cbrt(f32 x);
f32 hypotenuse(f32 x, f32 y);

f32 fastInvSqrt(f32 x);

// Exponential and Logarithm
f32 exp(f32 x);  // e^x
f32 exp2(f32 x); // 2^x
f32 ln(f32 x);
f32 ln1p(f32 x); // ln(1 + x)
f32 log2(f32 x);
f32 log10(f32 x);
f32 logBase(f32 x, f32 base);

// Rounding
f32 ceil(f32 x);
f32 floor(f32 x);
f32 mod(f32 x, f32 y);
f32 truncate(f32 x);
f32 round(f32 x);

s32 sign(s32 x);
s64 sign(s64 x);
f32 sign(f32 x);

// Other
f32 abs(f32 x);
s8 abs(s8 x);
s16 abs(s16 x);
s32 abs(s32 x);
s64 abs(s64 x);

Radian abs(const Radian& x);
Degree abs(const Degree& x);

template <typename T>
T clamp(const T& x, const T& lower, const T& upper)
{
	return std::max(lower, std::min(x, upper));
}

template <typename T>
inline T kroneckerDelta(const T& i, const T& j)
{
	return i == j ? T{1} : T{0};
}

////////////////////////////////
// Interpolation
////////////////////////////////

// Linear Interpolation
template <typename T>
inline T lerp(const T& x, const T& y, f32 t)
{
	assert(t >= 0.0f && t <= 1.0f && "Math::lerp `t` out range (0..1).");

	return x * (1.0f - t) + (y * t);
}

// Spherical Linear Interpolation
inline Quaternion slerp(const Quaternion& x, const Quaternion& y, f32 t)
{
	assert(t >= 0.0f && t <= 1.0f && "Math::lerp `t` out range (0..1).");

	Quaternion z = y;

	f32 cosTheta = dot(x, y);

	if (cosTheta < 0.0f)
	{
		z        = -y;
		cosTheta = -cosTheta;
	}

	if (cosTheta > 1.0f)
	{
		return Quaternion{lerp(x.x, y.x, t),
		                  lerp(x.y, y.y, t),
		                  lerp(x.z, y.z, t),
		                  lerp(x.w, y.w, t)};
	}

	Radian angle = Math::acos(cosTheta);

	Quaternion result =
	    Math::sin(Radian{1.0f} - (t * angle)) * x + Math::sin(t * angle) * z;
	return result * (1.0f / Math::sin(angle));
}

// Shoemake's Quaternion Curves
// Sqherical Cubic Interpolation
inline Quaternion squad(const Quaternion& p,
                        const Quaternion& a,
                        const Quaternion& b,
                        const Quaternion& q,
                        f32 t)
{
	return slerp(slerp(p, q, t), slerp(a, b, t), 2.0f * t * (1.0f - t));
}

////////////////////////////////
// Transformation
////////////////////////////////

Matrix4 translate(const Vector3& v);
Matrix4 rotate(const Radian& angle, const Vector3& v);
Matrix4 scale(const Vector3& v);

Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top);
Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);

Matrix4 perspective(const Radian& fovy, f32 aspect, f32 zNear, f32 zFar);
Matrix4 infinitePerspective(const Radian& fovy, f32 aspect, f32 zNear);

template <typename T>
T lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

template <>
Matrix4 lookAt<Matrix4>(const Vector3& eye,
                        const Vector3& center,
                        const Vector3& up);

template <>
Quaternion lookAt<Quaternion>(const Vector3& eye,
                              const Vector3& center,
                              const Vector3& up);
} // namespace Math
} // namespace Dunjun

#endif
