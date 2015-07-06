#ifndef DUNJUN_MATH_TYPES_HPP
#define DUNJUN_MATH_TYPES_HPP

#include <Dunjun/Common.hpp>

namespace Dunjun
{
struct Vector2
{
	union
	{
		struct
		{
			f32 x, y;
		};
		f32 data[2];
	};

	GLOBAL const Vector2 Zero;

	inline f32& operator[](usize index) { return data[index]; }
	inline const f32& operator[](usize index) const { return data[index]; }
};

struct Vector3
{
	union
	{
		struct
		{
			f32 x, y, z;
		};
		struct
		{
			f32 r, g, b;
		};
		f32 data[3];

		Vector2 xy;
	};

	GLOBAL const Vector3 Zero;

	inline f32& operator[](usize index) { return data[index]; }
	inline const f32& operator[](usize index) const { return data[index]; }
};

struct Vector4
{
	union
	{
		struct
		{
			f32 x, y, z, w;
		};
		struct
		{
			f32 r, g, b, a;
		};
		f32 data[4];

		struct
		{
			Vector2 xy;
			Vector2 zw;
		};

		struct
		{
			Vector2 rg;
			Vector2 ba;
		};

		Vector3 xyz;
		Vector3 rgb;
	};

	GLOBAL const Vector4 Zero;

	inline f32& operator[](usize index) { return data[index]; }
	inline const f32& operator[](usize index) const { return data[index]; }
};

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

struct Quaternion
{
	union
	{
		struct
		{
			f32 x, y, z, w;
		};
		Vector3 xyz;
		f32 data[4];
	};

	GLOBAL const Quaternion Identity;
};
} // namespace Dunjun

#endif
