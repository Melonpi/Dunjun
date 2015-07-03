#include <Dunjun/Math/Rectangle.hpp>

#include <Dunjun/Math/Functions.hpp>

namespace Dunjun
{
bool contains(const Rectangle& a, const Vector2& point)
{
	// TODO(bill): compare similar due to floating point arithmetic
	const f32 minX = std::min(a.x, a.x + a.width);
	const f32 maxX = std::max(a.x, a.x + a.width);
	const f32 minY = std::min(a.y, a.y + a.height);
	const f32 maxY = std::max(a.y, a.y + a.height);

	return (point.x >= minX) && (point.x < maxX) && (point.y >= minY) &&
	       (point.y < maxY);
}

bool intersects(const Rectangle& a, const Rectangle& b)
{
	Rectangle intersection;
	return intersects(a, b, intersection);
}

bool intersects(const Rectangle& a, const Rectangle& b, Rectangle& intersection)
{
	const f32 r1MinX = std::min(a.x, a.x + a.width);
	const f32 r1MaxX = std::max(a.x, a.x + a.width);
	const f32 r1MinY = std::min(a.y, a.y + a.height);
	const f32 r1MaxY = std::max(a.y, a.y + a.height);

	const f32 r2MinX = std::min(b.x, b.x + b.width);
	const f32 r2MaxX = std::max(b.x, b.x + b.width);
	const f32 r2MinY = std::min(b.y, b.y + b.height);
	const f32 r2MaxY = std::max(b.y, b.y + b.height);

	const f32 x1 = std::max(r1MinX, r2MinX);
	const f32 x2 = std::min(r1MaxX, r2MaxX);

	const f32 y1 = std::max(r1MinY, r2MinY);
	const f32 y2 = std::min(r1MaxY, r2MaxY);

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

bool operator==(const Rectangle& left, const Rectangle& right)
{
	// TODO(bill): compare similar due to floating point arithmetic
	return (left.x == right.x) && (left.y == right.y) &&
	       (left.width == right.width) && (left.height == right.height);
}

bool operator!=(const Rectangle& left, const Rectangle& right)
{
	return !(left == right);
}

} // namespace Dunjun
