#ifndef DUNJUN_MATH_MATRIX4_HPP
#define DUNJUN_MATH_MATRIX4_HPP

#include <Dunjun/Math/Types.hpp>
#include <ostream>

namespace Dunjun
{
bool operator==(const Matrix4& a, const Matrix4& b);
bool operator!=(const Matrix4& a, const Matrix4& b);

Matrix4 operator+(const Matrix4& a, const Matrix4& b);
Matrix4 operator-(const Matrix4& a, const Matrix4& b);

Matrix4 operator*(const Matrix4& a, const Matrix4& b);
Vector4 operator*(const Matrix4& a, const Vector4& v);
Matrix4 operator*(const Matrix4& a, f32 scalar);

Matrix4 operator/(const Matrix4& a, f32 scalar);

Matrix4& operator+=(Matrix4& a, const Matrix4& b);
Matrix4& operator-=(Matrix4& a, const Matrix4& b);

Matrix4& operator*=(Matrix4& a, const Matrix4& b);

inline Matrix4 operator*(f32 scalar, const Matrix4& m) { return m * scalar; }

Matrix4 transpose(const Matrix4& m);

f32 determinant(const Matrix4& m);

Matrix4 inverse(const Matrix4& m);

Matrix4 hadamardProduct(const Matrix4& a, const Matrix4& b);

inline std::ostream& operator<<(std::ostream& os, const Matrix4& m)
{
	os << "Matrix4{";
	for (usize i = 0; i < 4; i++)
	{
		os << "\n\t";
		for (usize j = 0; j < 4; j++)
		{
			os << m[i][j];
			if (j < 3)
				os << ", ";
		}
		if (i < 3)
			os << ", ";
	}
	os << "\n}";

	return os;
}

} // namespace Dunjun

#endif
