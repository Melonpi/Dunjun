#ifndef DUNJUN_MATH_MATRIX4_HPP
#define DUNJUN_MATH_MATRIX4_HPP

#include <Dunjun/Math/Vector4.hpp>

#include <Dunjun/Common.hpp>

namespace Dunjun
{
struct Matrix4
{
	union
	{
		struct
		{
			Vector4 x, y, z, w;
		};
		Vector4 data[4];
	};

	GLOBAL const Matrix4 Identity;

	inline Vector4& operator[](usize index) { return data[index]; }
	inline const Vector4& operator[](usize index) const { return data[index]; }
};

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
	os << "Matrix4(";
	for (usize i{0}; i < 4; i++)
	{
		os << "\n\t" << m[i];
		if (i < 3)
			os << ", ";
	}
	os << "\n)";

	return os;
}

} // namespace Dunjun

#endif
