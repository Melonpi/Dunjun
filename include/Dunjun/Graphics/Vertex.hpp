#ifndef DUNJUN_VERTEX_HPP
#define DUNJUN_VERTEX_HPP

#include <Dunjun/Math.hpp>
#include <Dunjun/Graphics/Color.hpp>

#include <vector>

namespace Dunjun
{
struct Vertex
{
	Vector3 position = Vector3{0, 0, 0};
	Vector2 texCoord = Vector2{0, 0};
	Color color = Color{255, 255, 255};
	Vector3 normal = Vector3{0, 0, 0};

	Vertex(const Vector3& position = {0, 0, 0},
	       const Vector2& texCoord = {0, 0},
	       const Color& color = Color{0xFFFFFFFF},
	       const Vector3& normal = {0, 0, 0})
	: position(position)
	, texCoord(texCoord)
	, color(color)
	, normal(normal)
	{
	}
};

class VertexArray
{
public:
	inline VertexArray& append(const Vertex& vertex)
	{
		m_vertices.emplace_back(vertex);

		return *this;
	}

	inline VertexArray& append(const Vector3& position = {0, 0, 0},
	                           const Vector2& texCoord = {0, 0},
	                           const Color& color = Color(0xFFFFFFFF),
	                           const Vector3& normal = {0, 0, 0})
	{
		return append(Vertex(position, texCoord, color, normal));
	}

	inline const Vertex& operator[](usize index) const
	{
		return m_vertices[index];
	}

	inline Vertex& operator[](usize index) { return m_vertices[index]; }

	inline usize size() const { return m_vertices.size(); }

	inline void reserve(usize size) { return m_vertices.reserve(size); }

private:
	std::vector<Vertex> m_vertices;
};

} // namespace Dunjun

#endif
