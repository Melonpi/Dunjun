#include <Dunjun/Math/Matrix4.hpp>

namespace Dunjun
{
const Matrix4 Matrix4::Identity = {Vector4{1, 0, 0, 0},
                                   Vector4{0, 1, 0, 0},
                                   Vector4{0, 0, 1, 0},
                                   Vector4{0, 0, 0, 1}};

bool operator==(const Matrix4& a, const Matrix4& b)
{
	for (usize i = 0; i < 4; i++)
	{
		if (a[i] != b[i])
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
	for (usize i = 0; i < 4; i++)
		mat[i] = a[i] + b[i];
	return mat;
}

Matrix4 operator-(const Matrix4& a, const Matrix4& b)
{
	Matrix4 mat;
	for (usize i = 0; i < 4; i++)
		mat[i] = a[i] - b[i];
	return mat;
}

Matrix4 operator*(const Matrix4& a, const Matrix4& b)
{

	Matrix4 result;
	result[0] = a[0] * b[0][0] + a[1] * b[0][1] + a[2] * b[0][2] + a[3] * b[0][3];
	result[1] = a[0] * b[1][0] + a[1] * b[1][1] + a[2] * b[1][2] + a[3] * b[1][3];
	result[2] = a[0] * b[2][0] + a[1] * b[2][1] + a[2] * b[2][2] + a[3] * b[2][3];
	result[3] = a[0] * b[3][0] + a[1] * b[3][1] + a[2] * b[3][2] + a[3] * b[3][3];
	return result;
}

Vector4 operator*(const Matrix4& a, const Vector4& v)
{

	const Vector4 mul0 = a[0] * v[0];
	const Vector4 mul1 = a[1] * v[1];
	const Vector4 mul2 = a[2] * v[2];
	const Vector4 mul3 = a[3] * v[3];

	const Vector4 add0 = mul0 + mul1;
	const Vector4 add1 = mul2 + mul3;

	return add0 + add1;
}

Matrix4 operator*(const Matrix4& a, f32 scalar)
{
	Matrix4 mat;
	for (usize i = 0; i < 4; i++)
		mat[i] = a[i] * scalar;
	return mat;
}

Matrix4 operator/(const Matrix4& a, f32 scalar)
{
	Matrix4 mat;
	for (usize i = 0; i < 4; i++)
		mat[i] = a[i] / scalar;
	return mat;
}

Matrix4& operator+=(Matrix4& a, const Matrix4& b) { return (a = a + b); }

Matrix4& operator-=(Matrix4& a, const Matrix4& b) { return (a = a - b); }

Matrix4& operator*=(Matrix4& a, const Matrix4& b) { return (a = a * b); }

Matrix4 transpose(const Matrix4& m)
{
	Matrix4 result;

	for (usize i = 0; i < 4; i++)
	{
		for (usize j = 0; j < 4; j++)
			result[i][j] = m[j][i];
	}
	return result;
}

f32 determinant(const Matrix4& m)
{
	const f32 coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	const f32 coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	const f32 coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

	const f32 coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	const f32 coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	const f32 coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

	const f32 coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	const f32 coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	const f32 coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

	const f32 coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	const f32 coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	const f32 coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

	const f32 coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	const f32 coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	const f32 coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

	const f32 coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	const f32 coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	const f32 coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	const Vector4 fac0 = {coef00, coef00, coef02, coef03};
	const Vector4 fac1 = {coef04, coef04, coef06, coef07};
	const Vector4 fac2 = {coef08, coef08, coef10, coef11};
	const Vector4 fac3 = {coef12, coef12, coef14, coef15};
	const Vector4 fac4 = {coef16, coef16, coef18, coef19};
	const Vector4 fac5 = {coef20, coef20, coef22, coef23};

	const Vector4 vec0 = {m[1][0], m[0][0], m[0][0], m[0][0]};
	const Vector4 vec1 = {m[1][1], m[0][1], m[0][1], m[0][1]};
	const Vector4 vec2 = {m[1][2], m[0][2], m[0][2], m[0][2]};
	const Vector4 vec3 = {m[1][3], m[0][3], m[0][3], m[0][3]};

	const Vector4 inv0 = vec1 * fac0 - vec2 * fac1 + vec3 * fac2;
	const Vector4 inv1 = vec0 * fac0 - vec2 * fac3 + vec3 * fac4;
	const Vector4 inv2 = vec0 * fac1 - vec1 * fac3 + vec3 * fac5;
	const Vector4 inv3 = vec0 * fac2 - vec1 * fac4 + vec2 * fac5;

	const Vector4 signA = {+1, -1, +1, -1};
	const Vector4 signB = {-1, +1, -1, +1};
	const Matrix4 inverse = {inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB};

	const Vector4 row0 = {inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]};

	const Vector4 dot0 = m[0] * row0;
	const f32 dot1 = (dot0[0] + dot0[1]) + (dot0[2] + dot0[3]);
	return dot1;
}

Matrix4 inverse(const Matrix4& m)
{
	const f32 coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	const f32 coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	const f32 coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
	const f32 coef04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	const f32 coef06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	const f32 coef07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];
	const f32 coef08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	const f32 coef10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	const f32 coef11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];
	const f32 coef12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	const f32 coef14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	const f32 coef15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];
	const f32 coef16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	const f32 coef18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	const f32 coef19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];
	const f32 coef20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	const f32 coef22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	const f32 coef23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	const Vector4 fac0 = {coef00, coef00, coef02, coef03};
	const Vector4 fac1 = {coef04, coef04, coef06, coef07};
	const Vector4 fac2 = {coef08, coef08, coef10, coef11};
	const Vector4 fac3 = {coef12, coef12, coef14, coef15};
	const Vector4 fac4 = {coef16, coef16, coef18, coef19};
	const Vector4 fac5 = {coef20, coef20, coef22, coef23};

	const Vector4 vec0 = {m[1][0], m[0][0], m[0][0], m[0][0]};
	const Vector4 vec1 = {m[1][1], m[0][1], m[0][1], m[0][1]};
	const Vector4 vec2 = {m[1][2], m[0][2], m[0][2], m[0][2]};
	const Vector4 vec3 = {m[1][3], m[0][3], m[0][3], m[0][3]};

	const Vector4 inv0 = vec1 * fac0 - vec2 * fac1 + vec3 * fac2;
	const Vector4 inv1 = vec0 * fac0 - vec2 * fac3 + vec3 * fac4;
	const Vector4 inv2 = vec0 * fac1 - vec1 * fac3 + vec3 * fac5;
	const Vector4 inv3 = vec0 * fac2 - vec1 * fac4 + vec2 * fac5;

	const Vector4 signA = {+1, -1, +1, -1};
	const Vector4 signB = {-1, +1, -1, +1};
	const Matrix4 inverse = {inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB};

	const Vector4 row0 = {inverse[0][0], inverse[1][0], inverse[2][0], inverse[3][0]};

	const Vector4 dot0 = m[0] * row0;
	const f32 dot1 = (dot0[0] + dot0[1]) + (dot0[2] + dot0[3]);

	const f32 oneOverDeterminant = 1.0f / dot1;

	return inverse * oneOverDeterminant;
}

Matrix4 hadamardProduct(const Matrix4& a, const Matrix4& b)
{
	Matrix4 result;

	for (usize i = 0; i < 4; i++)
		result[i] = a[i] * b[i];

	return result;
}

} // namespace  Dunjun
