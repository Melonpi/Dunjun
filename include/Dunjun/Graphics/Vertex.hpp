#ifndef DUNJUN_VERTEX_HPP
#define DUNJUN_VERTEX_HPP

#include <Dunjun/Math.hpp>
#include <Dunjun/Graphics/Color.hpp>

namespace Dunjun
{
struct Vertex
{
	Vector3 position = Vector3{0, 0, 0};
	Vector2 texCoord = Vector2{0, 0};
	Color color      = Color{255, 255, 255, 255};
	Vector3 normal   = Vector3{0, 0, 0};

	Vertex(const Vector3& position = Vector3{0, 0, 0},
	       const Vector2& texCoord = Vector2{0, 0},
	       const Color& color      = {255, 255, 255, 255},
	       const Vector3& normal   = Vector3{0, 0, 0})
	: position(position)
	, texCoord(texCoord)
	, color(color)
	, normal(normal)
	{
	}
};
} // namespace Dunjun

#endif
