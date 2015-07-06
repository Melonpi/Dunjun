#ifndef DUNJUN_MATH_RECT_HPP
#define DUNJUN_MATH_RECT_HPP

#include <Dunjun/Math/Types.hpp>

namespace Dunjun
{
struct Rectangle
{
	f32 x      = 0;
	f32 y      = 0;
	f32 width  = 0;
	f32 height = 0;
};

bool operator==(const Rectangle& left, const Rectangle& right);
bool operator!=(const Rectangle& left, const Rectangle& right);

bool contains(const Rectangle& a, const Vector2& point);
bool intersects(const Rectangle& a, const Rectangle& b);
bool intersects(const Rectangle& a,
                const Rectangle& b,
                Rectangle& intersection);

} // namespace Dunjun

#endif
