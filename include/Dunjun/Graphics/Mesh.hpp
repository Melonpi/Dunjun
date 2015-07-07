#ifndef DUNJUN_GRAPHICS_MESH_HPP
#define DUNJUN_GRAPHICS_MESH_HPP

#include <Dunjun/System/OpenGL.hpp>
#include <Dunjun/Common.hpp>
#include <Dunjun/Graphics/Vertex.hpp>

#include <Dunjun/System/Containers.hpp>

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

struct MeshData
{
	DrawType drawType;

	Array<Vertex> vertices;
	Array<u32> indices;

	MeshData(Allocator& a);
	MeshData(const MeshData& other) = default;
	~MeshData() = default;

	MeshData& addFace(u32 a, u32 b, u32 c);
	MeshData& addFace(u32 offset, u32 a, u32 b, u32 c);

	void generateNormals();
};

struct Mesh
{
	u32 vbo;
	u32 ibo;
	DrawType drawType;
	s32 drawCount;
};

Mesh generateMesh(const MeshData& data);

void drawMesh(const Mesh& mesh);

inline void destroyMesh(Mesh& mesh)
{
	glDeleteBuffers(1, &mesh.vbo);
	glDeleteBuffers(1, &mesh.ibo);
}

} // namespace Dunjun

#endif
