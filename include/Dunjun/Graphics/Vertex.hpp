#ifndef DUNJUN_VERTEX_HPP
#define DUNJUN_VERTEX_HPP

#include <Dunjun/Math.hpp>
#include <Dunjun/Graphics/Color.hpp>

namespace Dunjun
{
struct Vertex
{
	Vector3 position = Vector3::Zero;
	Vector2 texCoord = Vector2::Zero;
	Color color      = Color::White;
	Vector3 normal   = Vector3::Zero;
	Vector3 tangent  = Vector3::Zero;

	Vertex(const Vector3& position = Vector3::Zero,
	       const Vector2& texCoord = Vector2::Zero,
	       const Color& color      = Color::White,
	       const Vector3& normal   = Vector3::Zero,
	       const Vector3& tangent  = Vector3::Zero)
	: position(position)
	, texCoord(texCoord)
	, color(color)
	, normal(normal)
	, tangent(tangent)
	{
	}
};
} // namespace Dunjun

#endif
