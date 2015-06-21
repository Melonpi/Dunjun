#include <Dunjun/Math/Matrix4.hpp>

namespace Dunjun
{
const Matrix4 Matrix4::Identity = {Vector4{1, 0, 0, 0},
                                   Vector4{0, 1, 0, 0},
                                   Vector4{0, 0, 1, 0},
                                   Vector4{0, 0, 0, 1}};

bool operator==(const Matrix4& a, const Matrix4& b)
{
	for (usize i{0}; i < 4; i++)
	{
		if (a.data[i] != b.data[i])
			return false;
	}
	return true;
}

bool operator!=(const Matrix4& a, const Matrix4& b)
{
	return !operator==(a, b);
}

Matrix4 operator+(const Matrix4& a, const Matrix4& b)
{
	Matrix4 mat;
	for (usize i{0}; i < 4; i++)
		mat.data[i] = a.data[i] + b.data[i];
	return mat;
}

Matrix4 operator-(const Matrix4& a, const Matrix4& b)
{
	Matrix4 mat;
	for (usize i{0}; i < 4; i++)
		mat.data[i] = a.data[i] - b.data[i];
	return mat;
}

Matrix4 operator*(const Matrix4& a, const Matrix4& b)
{

	Matrix4 result;
	result.x = a.x * b.x.x + a.y * b.x.y + a.z * b.x.z + a.w * b.x.w;
	result.y = a.x * b.y.x + a.y * b.y.y + a.z * b.y.z + a.w * b.y.w;
	result.z = a.x * b.z.x + a.y * b.z.y + a.z * b.z.z + a.w * b.z.w;
	result.w = a.x * b.w.x + a.y * b.w.y + a.z * b.w.z + a.w * b.w.w;
	return result;
}

Vector4 operator*(const Matrix4& a, const Vector4& v)
{

	const Vector4 mul0 = a.x * v.x;
	const Vector4 mul1 = a.y * v.y;
	const Vector4 mul2 = a.z * v.z;
	const Vector4 mul3 = a.w * v.w;

	const Vector4 add0 = mul0 + mul1;
	const Vector4 add1 = mul2 + mul3;

	return add0 + add1;
}

Matrix4 operator*(const Matrix4& a, f32 scalar)
{
	Matrix4 mat;
	for (usize i{0}; i < 4; i++)
		mat.data[i] = a.data[i] * scalar;
	return mat;
}

Matrix4 operator/(const Matrix4& a, f32 scalar)
{
	Matrix4 mat;
	for (usize i{0}; i < 4; i++)
		mat.data[i] = a.data[i] / scalar;
	return mat;
}

Matrix4& operator+=(Matrix4& a, const Matrix4& b) { return (a = a + b); }

Matrix4& operator-=(Matrix4& a, const Matrix4& b) { return (a = a - b); }

Matrix4& operator*=(Matrix4& a, const Matrix4& b) { return (a = a * b); }

Matrix4 transpose(const Matrix4& m)
{
	Matrix4 result;

	for (usize i{0}; i < 4; i++)
	{
		for (usize j{0}; j < 4; j++)
			result.data[i].data[j] = m.data[j].data[i];
	}
	return result;
}

f32 determinant(const Matrix4& m)
{
	f32 coef00{m.z.z * m.w.w - m.w.z * m.z.w};
	f32 coef02{m.y.z * m.w.w - m.w.z * m.y.w};
	f32 coef03{m.y.z * m.z.w - m.z.z * m.y.w};

	f32 coef04{m.z.y * m.w.w - m.w.y * m.z.w};
	f32 coef06{m.y.y * m.w.w - m.w.y * m.y.w};
	f32 coef07{m.y.y * m.z.w - m.z.y * m.y.w};

	f32 coef08{m.z.y * m.w.z - m.w.y * m.z.z};
	f32 coef10{m.y.y * m.w.z - m.w.y * m.y.z};
	f32 coef11{m.y.y * m.z.z - m.z.y * m.y.z};

	f32 coef12{m.z.x * m.w.w - m.w.x * m.z.w};
	f32 coef14{m.y.x * m.w.w - m.w.x * m.y.w};
	f32 coef15{m.y.x * m.z.w - m.z.x * m.y.w};

	f32 coef16{m.z.x * m.w.z - m.w.x * m.z.z};
	f32 coef18{m.y.x * m.w.z - m.w.x * m.y.z};
	f32 coef19{m.y.x * m.z.z - m.z.x * m.y.z};

	f32 coef20{m.z.x * m.w.y - m.w.x * m.z.y};
	f32 coef22{m.y.x * m.w.y - m.w.x * m.y.y};
	f32 coef23{m.y.x * m.z.y - m.z.x * m.y.y};

	Vector4 fac0 = {coef00, coef00, coef02, coef03};
	Vector4 fac1 = {coef04, coef04, coef06, coef07};
	Vector4 fac2 = {coef08, coef08, coef10, coef11};
	Vector4 fac3 = {coef12, coef12, coef14, coef15};
	Vector4 fac4 = {coef16, coef16, coef18, coef19};
	Vector4 fac5 = {coef20, coef20, coef22, coef23};

	Vector4 vec0 = {m.y.x, m.x.x, m.x.x, m.x.x};
	Vector4 vec1 = {m.y.y, m.x.y, m.x.y, m.x.y};
	Vector4 vec2 = {m.y.z, m.x.z, m.x.z, m.x.z};
	Vector4 vec3 = {m.y.w, m.x.w, m.x.w, m.x.w};

	Vector4 inv0 = vec1 * fac0 - vec2 * fac1 + vec3 * fac2;
	Vector4 inv1 = vec0 * fac0 - vec2 * fac3 + vec3 * fac4;
	Vector4 inv2 = vec0 * fac1 - vec1 * fac3 + vec3 * fac5;
	Vector4 inv3 = vec0 * fac2 - vec1 * fac4 + vec2 * fac5;

	Vector4 signA = {+1, -1, +1, -1};
	Vector4 signB = {-1, +1, -1, +1};
	Matrix4 inverse = {inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB};

	Vector4 row0 = {inverse.x.x, inverse.y.x, inverse.z.x, inverse.w.x};

	Vector4 dot0 = m.x * row0;
	f32 dot1{(dot0.x + dot0.y) + (dot0.z + dot0.w)};
	return dot1;
}

Matrix4 inverse(const Matrix4& m)
{
	f32 coef00{m.z.z * m.w.w - m.w.z * m.z.w};
	f32 coef02{m.y.z * m.w.w - m.w.z * m.y.w};
	f32 coef03{m.y.z * m.z.w - m.z.z * m.y.w};
	f32 coef04{m.z.y * m.w.w - m.w.y * m.z.w};
	f32 coef06{m.y.y * m.w.w - m.w.y * m.y.w};
	f32 coef07{m.y.y * m.z.w - m.z.y * m.y.w};
	f32 coef08{m.z.y * m.w.z - m.w.y * m.z.z};
	f32 coef10{m.y.y * m.w.z - m.w.y * m.y.z};
	f32 coef11{m.y.y * m.z.z - m.z.y * m.y.z};
	f32 coef12{m.z.x * m.w.w - m.w.x * m.z.w};
	f32 coef14{m.y.x * m.w.w - m.w.x * m.y.w};
	f32 coef15{m.y.x * m.z.w - m.z.x * m.y.w};
	f32 coef16{m.z.x * m.w.z - m.w.x * m.z.z};
	f32 coef18{m.y.x * m.w.z - m.w.x * m.y.z};
	f32 coef19{m.y.x * m.z.z - m.z.x * m.y.z};
	f32 coef20{m.z.x * m.w.y - m.w.x * m.z.y};
	f32 coef22{m.y.x * m.w.y - m.w.x * m.y.y};
	f32 coef23{m.y.x * m.z.y - m.z.x * m.y.y};

	Vector4 fac0{coef00, coef00, coef02, coef03};
	Vector4 fac1{coef04, coef04, coef06, coef07};
	Vector4 fac2{coef08, coef08, coef10, coef11};
	Vector4 fac3{coef12, coef12, coef14, coef15};
	Vector4 fac4{coef16, coef16, coef18, coef19};
	Vector4 fac5{coef20, coef20, coef22, coef23};

	Vector4 vec0{m.y.x, m.x.x, m.x.x, m.x.x};
	Vector4 vec1{m.y.y, m.x.y, m.x.y, m.x.y};
	Vector4 vec2{m.y.z, m.x.z, m.x.z, m.x.z};
	Vector4 vec3{m.y.w, m.x.w, m.x.w, m.x.w};

	Vector4 inv0 = vec1 * fac0 - vec2 * fac1 + vec3 * fac2;
	Vector4 inv1 = vec0 * fac0 - vec2 * fac3 + vec3 * fac4;
	Vector4 inv2 = vec0 * fac1 - vec1 * fac3 + vec3 * fac5;
	Vector4 inv3 = vec0 * fac2 - vec1 * fac4 + vec2 * fac5;

	Vector4 signA{+1, -1, +1, -1};
	Vector4 signB{-1, +1, -1, +1};
	Matrix4 inverse{inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB};

	Vector4 row0{inverse.x.x, inverse.y.x, inverse.z.x, inverse.w.x};

	Vector4 dot0 = m.x * row0;
	f32 dot1{(dot0.x + dot0.y) + (dot0.z + dot0.w)};

	f32 oneOverDeterminant{1.0f / dot1};

	return inverse * oneOverDeterminant;
}

Matrix4 hadamardProduct(const Matrix4& a, const Matrix4& b)
{
	Matrix4 result;

	for (usize i{0}; i < 4; i++)
		result.data[i] = a.data[i] * b.data[i];

	return result;
}

} // namespace  Dunjun
