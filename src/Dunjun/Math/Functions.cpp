#include <Dunjun/Math/Functions.hpp>
#include <Dunjun/Math/Matrix4.hpp>
#include <Dunjun/Math/Quaternion.hpp>

#include <cmath>
#include <cfenv>

namespace Dunjun
{
namespace Math
{
// Trigonometric
f32 sin(const Radian& theta) { return std::sin(static_cast<f32>(theta)); }
f32 cos(const Radian& theta) { return std::cos(static_cast<f32>(theta)); }
f32 tan(const Radian& theta) { return std::tan(static_cast<f32>(theta)); }

Radian asin(f32 a) { return Radian(std::asin(a)); }
Radian acos(f32 a) { return Radian(std::acos(a)); }
Radian atan(f32 a) { return Radian(std::atan(a)); }
Radian atan2(f32 y, f32 x) { return Radian(std::atan2(y, x)); }

// Hyperbolic
f32 sinh(f32 x) { return std::sinh(x); }
f32 cosh(f32 x) { return std::cosh(x); }
f32 tanh(f32 x) { return std::tanh(x); }

f32 asinh(f32 x) { return std::asinh(x); }
f32 acosh(f32 x) { return std::acosh(x); }
f32 atanh(f32 x) { return std::atanh(x); }

// Power
f32 pow(f32 x, f32 y) { return std::pow(x, y); }

f32 sqrt(f32 x) { return std::sqrt(x); }

f32 cbrt(f32 x) { return std::cbrt(x); }

f32 hypotenuse(f32 x, f32 y) { return std::hypot(x, y); }

f32 fastInvSqrt(f32 number)
{
	const f32 threeHalfs = 1.5f;

	f32 x2 = number * 0.5f;
	f32 y  = number;
	u32 i  = pseudo_cast<u32>(y); // Evil floating point bit level hacking
	//	i = 0x5f3759df - (i >> 1);           // What the fuck?
	i = 0x5f375a86 - (i >> 1); // What the fuck? Improved!
	y = pseudo_cast<f32>(i);
	y = y * (threeHalfs - (x2 * y * y)); // 1st iteration
	//	y = y * (threeHalfs - (x2 * y * y)); // 2nd iteration, this can be
	// removed

	return y;
}

// Exponential and Logarithm
f32 exp(f32 x) // e^x
{
	return std::exp(x);
}
f32 exp2(f32 x) // 2^x
{
	return std::exp2(x);
}
f32 ln(f32 x) { return std::log(x); }
f32 ln1p(f32 x) // ln(1 + x)
{
	return std::log1p(x);
}
f32 log2(f32 x) { return std::log2(x); }

f32 log10(f32 x) { return std::log10(x); }
f32 logBase(f32 x, f32 base) { return ln(x) * (1.0f / ln(base)); }

// Rounding
f32 ceil(f32 x) { return std::ceil(x); }
f32 floor(f32 x) { return std::floor(x); }
f32 mod(f32 x, f32 y)
{
	f32 result = std::remainder(Math::abs(x), (y = Math::abs(y)));
	if (std::signbit(result))
		result += y;
	return std::copysign(result, x);
}
f32 truncate(f32 x)
{
	u32 i              = pseudo_cast<u32>(x);
	u32 exponent       = (i >> 23) & 0xFF; // extract exponent field
	u32 fractionalBits = 0x7F + 23 - exponent;
	if (fractionalBits > 23) // abs(x) < 1.0f
		return 0.0f;
	if (fractionalBits > 0)
		i &= ~((1u << fractionalBits) - 1);
	return pseudo_cast<f32>(i);
}
f32 round(f32 x)
{
	std::fenv_t saveEnv;
	std::feholdexcept(&saveEnv);
	f32 result = std::rint(x);
	if (std::fetestexcept(FE_INEXACT))
	{
		std::fesetround(FE_TOWARDZERO);
		result = std::rint(std::copysign(0.5f + Math::abs(x), x));
	}
	std::feupdateenv(&saveEnv);
	return result;
}

s32 sign(s32 x)
{
	u32 i = reinterpret_cast<const u32&>(x);
	i &= 0x80000000ul;
	if (i)
		return -1;
	return 1;
}

s64 sign(s64 x)
{
	u64 i = reinterpret_cast<const u64&>(x);
	i &= 0x8000000000000000ull;
	if (i)
		return -1;
	return 1;
}

f32 sign(f32 x)
{
	u32 i = reinterpret_cast<const u32&>(x);
	i &= 0x80000000ul;
	if (i)
		return -1.0f;
	return 1.0f;
}

// Other
f32 abs(f32 x)
{
	u32 i = reinterpret_cast<const u32&>(x);
	i &= 0x7FFFFFFFul;
	return reinterpret_cast<const f32&>(i);
}
s8 abs(s8 x)
{
	u8 i = reinterpret_cast<const u8&>(x);
	i &= 0x7Fu;
	return reinterpret_cast<const s8&>(i);
}
s16 abs(s16 x)
{
	u16 i = reinterpret_cast<const u16&>(x);
	i &= 0x7FFFu;
	return reinterpret_cast<const s16&>(i);
}
s32 abs(s32 x)
{
	u32 i = reinterpret_cast<const u32&>(x);
	i &= 0x7FFFFFFFul;
	return reinterpret_cast<const s32&>(i);
}
s64 abs(s64 x)
{
	u64 i = reinterpret_cast<const u64&>(x);
	i &= 0x7FFFFFFFFFFFFFFFull;
	return reinterpret_cast<const s64&>(i);
}

Radian abs(const Radian& x) { return Radian(abs(static_cast<f32>(x))); }

Degree abs(const Degree& x) { return Degree(abs(static_cast<f32>(x))); }

Matrix4 translate(const Vector3& v)
{
	Matrix4 result = Matrix4::Identity;
	result[3].xyz  = v;
	result[3].w    = 1;
	return result;
}

// Angle in Radians
Matrix4 rotate(const Radian& angle, const Vector3& v)
{
	const f32 c = Math::cos(angle);
	const f32 s = Math::sin(angle);

	const Vector3 axis = normalize(v);
	const Vector3 t    = (1.0f - c) * axis;

	Matrix4 rot = Matrix4::Identity;
	rot[0][0]   = c + t.x * axis.x;
	rot[0][1]   = 0 + t.x * axis.y + s * axis.z;
	rot[0][2]   = 0 + t.x * axis.z - s * axis.y;
	rot[0][3]   = 0;

	rot[1][0] = 0 + t.y * axis.x - s * axis.z;
	rot[1][1] = c + t.y * axis.y;
	rot[1][2] = 0 + t.y * axis.z + s * axis.x;
	rot[1][3] = 0;

	rot[2][0] = 0 + t.z * axis.x + s * axis.y;
	rot[2][1] = 0 + t.z * axis.y - s * axis.x;
	rot[2][2] = c + t.z * axis.z;
	rot[2][3] = 0;

	return rot;
}

Matrix4 scale(const Vector3& v)
{
	return Matrix4{Vector4{v.x, 0, 0, 0},
	               Vector4{0, v.y, 0, 0},
	               Vector4{0, 0, v.z, 0},
	               Vector4{0, 0, 0, 1}};
}

Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top)
{
	Matrix4 result = Matrix4::Identity;

	result[0][0] = 2.0f / (right - left);
	result[1][1] = 2.0f / (top - bottom);
	result[2][2] = -1.0f;
	result[3][1] = -(right + left) / (right - left);
	result[3][1] = -(top + bottom) / (top - bottom);

	return result;
}

Matrix4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
{
	Matrix4 result = Matrix4::Identity;

	result[0][0] = 2.0f / (right - left);
	result[1][1] = 2.0f / (top - bottom);
	result[2][2] = -2.0f / (zFar - zNear);
	result[3][0] = -(right + left) / (right - left);
	result[3][1] = -(top + bottom) / (top - bottom);
	result[3][2] = -(zFar + zNear) / (zFar - zNear);

	return result;
}

Matrix4 perspective(const Radian& fovy, f32 aspect, f32 zNear, f32 zFar)
{
	assert(Math::abs(aspect - std::numeric_limits<f32>::epsilon()) > 0.0f &&
	       "Math::perspective `fovy` is 0/inf.");

	const f32 tanHalfFovy = Math::tan(0.5f * fovy);

	Matrix4 result = 0.0f * Matrix4::Identity;
	result[0][0]   = 1.0f / (aspect * tanHalfFovy);
	result[1][1]   = 1.0f / (tanHalfFovy);
	result[2][2]   = -(zFar + zNear) / (zFar - zNear);
	result[2][3]   = -1.0f;
	result[3][2]   = -2.0f * zFar * zNear / (zFar - zNear);

	return result;
}

Matrix4 infinitePerspective(const Radian& fovy, f32 aspect, f32 zNear)
{
	const f32 range  = Math::tan(0.5f * fovy) * zNear;
	const f32 left   = -range * aspect;
	const f32 right  = range * aspect;
	const f32 bottom = -range;
	const f32 top    = range;

	Matrix4 result = 0.0f * Matrix4::Identity;

	result[0][0] = (2.0f * zNear) / (right - left);
	result[1][1] = (2.0f * zNear) / (top - bottom);
	result[2][2] = -1.0f;
	result[2][3] = -1.0f;
	result[3][2] = -2.0f * zNear;

	return result;
}

template <>
Matrix4 lookAt<Matrix4>(const Vector3& eye,
                        const Vector3& center,
                        const Vector3& up)
{
	const Vector3 f = normalize(center - eye);
	const Vector3 s = normalize(cross(f, up));
	const Vector3 u = cross(s, f);

	Matrix4 result = Matrix4::Identity;
	result[0][0]   = +s.x;
	result[1][0]   = +s.y;
	result[2][0]   = +s.z;

	result[0][1] = +u.x;
	result[1][1] = +u.y;
	result[2][1] = +u.z;

	result[0][2] = -f.x;
	result[1][2] = -f.y;
	result[2][2] = -f.z;

	result[3][0] = -dot(s, eye);
	result[3][1] = -dot(u, eye);
	result[3][2] = +dot(f, eye);

	return result;
}

// TODO(bill): properly implement quaternionLookAt
template <>
Quaternion lookAt<Quaternion>(const Vector3& eye,
                              const Vector3& center,
                              const Vector3& up)
{
	const f32 similar = 0.001f;

	if (length(center - eye) < similar)
		return Quaternion{0, 0, 0, 1}; // You cannot look at where you are!

	return matrix4ToQuaternion(lookAt<Matrix4>(eye, center, up));

	// const Vector3 f{normalize(center - eye)};
	// const Vector3 s{normalize(cross(f, up))};
	// const Vector3 u{cross(s, f)};
	// const Vector3 refUp{normalize(up)};

	//// NOTE(bill): this is from
	////
	/// http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
	// const f32 m{Math::sqrt(2.0f + 2.0f * dot(u, refUp))};
	// Vector3 v{(1.0f / m) * cross(u, refUp)};
	// return {v, 0.5f * m};
}
} // namespace Math
} // namespace Dunjun
