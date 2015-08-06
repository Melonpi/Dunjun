#ifndef DUNJUN_MATH_RECT_HPP
#define DUNJUN_MATH_RECT_HPP

#include <Dunjun/Math/Types.hpp>

namespace Dunjun
{
struct Rect
{
	f32 x;
	f32 y;
	f32 width;
	f32 height;
};

bool operator==(const Rect& left, const Rect& right);
bool operator!=(const Rect& left, const Rect& right);

bool contains(const Rect& a, const Vector2& point);
bool intersects(const Rect& a, const Rect& b);
bool intersects(const Rect& a, const Rect& b, Rect& intersection);

} // namespace Dunjun

#endif
