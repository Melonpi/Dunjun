#ifndef DUNJUN_GRAPHICS_MESH_HPP
#define DUNJUN_GRAPHICS_MESH_HPP

#include <Dunjun/System/OpenGL.hpp>
#include <Dunjun/Common.hpp>
#include <Dunjun/Graphics/Vertex.hpp>

#include <vector>

namespace Dunjun
{
class SceneRenderer;
enum class AtrribLocation : u32
{
	Position = 0,
	TexCoord = 1,
	Color    = 2,
	Normal   = 3,
};

enum class DrawType : s32
{
	Points        = GL_POINTS,
	Lines         = GL_LINES,
	LineStrip     = GL_LINE_STRIP,
	LineLoop      = GL_LINE_LOOP,
	Triangles     = GL_TRIANGLES,
	TriangleStrip = GL_TRIANGLE_STRIP,
	TriangleFan   = GL_TRIANGLE_FAN,
	// Quads      = GL_QUADS, // Disabled by default
};

struct Mesh
{
	struct Data
	{
		DrawType drawType{DrawType::Triangles};

		VertexArray vertices;
		std::vector<u32> indices;

		Data& addFace(u32 a, u32 b, u32 c)
		{
			indices.emplace_back(a);
			indices.emplace_back(b);
			indices.emplace_back(c);

			return *this;
		}

		Data& addFace(u32 offset, u32 a, u32 b, u32 c)
		{
			indices.emplace_back(offset + a);
			indices.emplace_back(offset + b);
			indices.emplace_back(offset + c);

			return *this;
		}

		void generateNormals();
	};

	Data data;
	// NOTE(bill): These mutables are a little bit of a hack but it works and
	// is semi-const correct

	mutable u32 vbo{0};
	mutable u32 ibo{0};

	mutable bool generated{false};

	DrawType drawType{DrawType::Triangles};
	s32 drawCount{0};

	Mesh();
	Mesh(const Data& data);

	virtual ~Mesh() { destroy(); }

	void addData(const Data& data);

	void generate() const;

	void draw() const;

	inline void destroy() const
	{
		if (vbo)
			glDeleteBuffers(1, &vbo);
		if (ibo)
			glDeleteBuffers(1, &ibo);
	}

};
} // namespace Dunjun

#endif
