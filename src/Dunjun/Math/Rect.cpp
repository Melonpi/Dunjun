#include <Dunjun/Math/Rect.hpp>

#include <Dunjun/Math/Functions.hpp>

namespace Dunjun
{
bool contains(const Rect& a, const Vector2& point)
{
	// TODO(bill): compare similar due to floating point arithmetic
	const f32 minX = Math::min(a.x, a.x + a.width);
	const f32 maxX = Math::max(a.x, a.x + a.width);
	const f32 minY = Math::min(a.y, a.y + a.height);
	const f32 maxY = Math::max(a.y, a.y + a.height);

	return (point.x >= minX) && (point.x < maxX) && (point.y >= minY) &&
	       (point.y < maxY);
}

bool intersects(const Rect& a, const Rect& b)
{
	Rect intersection;
	return intersects(a, b, intersection);
}

bool intersects(const Rect& a, const Rect& b, Rect& intersection)
{
	const f32 r1MinX = Math::min(a.x, a.x + a.width);
	const f32 r1MaxX = Math::max(a.x, a.x + a.width);
	const f32 r1MinY = Math::min(a.y, a.y + a.height);
	const f32 r1MaxY = Math::max(a.y, a.y + a.height);

	const f32 r2MinX = Math::min(b.x, b.x + b.width);
	const f32 r2MaxX = Math::max(b.x, b.x + b.width);
	const f32 r2MinY = Math::min(b.y, b.y + b.height);
	const f32 r2MaxY = Math::max(b.y, b.y + b.height);

	const f32 x1 = Math::max(r1MinX, r2MinX);
	const f32 x2 = Math::min(r1MaxX, r2MaxX);

	const f32 y1 = Math::max(r1MinY, r2MinY);
	const f32 y2 = Math::min(r1MaxY, r2MaxY);

	// If intersection is valid (positive non-zero area)
	if ((x1 < x2) && (y1 < y2))
	{
		intersection.x      = x1;
		intersection.y      = y1;
		intersection.width  = x2 - x1;
		intersection.height = y2 - y1;
		return true;
	}
	else
	{
		intersection.x      = 0;
		intersection.y      = 0;
		intersection.width  = 0;
		intersection.height = 0;

		return false;
	}
}

bool operator==(const Rect& left, const Rect& right)
{
	// TODO(bill): compare similar due to floating point arithmetic
	return (left.x == right.x) && (left.y == right.y) &&
	       (left.width == right.width) && (left.height == right.height);
}

bool operator!=(const Rect& left, const Rect& right)
{
	return !(left == right);
}

} // namespace Dunjun
