#include <Dunjun/Graphics/Mesh.hpp>

#include <Dunjun/Core/Memory.hpp>
#include <Dunjun/Core/Array.hpp>

namespace Dunjun
{
MeshData::MeshData(Allocator& a)
: drawType{DrawType::Triangles}
, vertices{a}
, indices{a}
{
}

MeshData& MeshData::addFace(u32 a, u32 b, u32 c)
{
	append(indices, a);
	append(indices, b);
	append(indices, c);

	return *this;
}

MeshData& MeshData::addFace(u32 offset, u32 a, u32 b, u32 c)
{
	append(indices, offset + a);
	append(indices, offset + b);
	append(indices, offset + c);

	return *this;
}

void MeshData::generateNormals()
{
	usize li = len(indices);
	for (usize i = 0; i < li; i += 3)
	{
		Vertex& v0 = vertices[indices[i + 0]];
		Vertex& v1 = vertices[indices[i + 1]];
		Vertex& v2 = vertices[indices[i + 2]];

		const Vector3 pos0 = v1.position - v0.position;
		const Vector3 pos1 = v2.position - v1.position;

		const Vector2 st0 = v1.texCoord - v0.texCoord;
		const Vector2 st1 = v2.texCoord - v1.texCoord;

		const Vector3 normal = normalize(cross(pos0, pos1));

		const f32 c = 1.0f / (st0.x * st1.y - st0.y * st1.x);
		const Vector3 tangent = normalize((pos0 * st1.y - pos1 * st1.y) * c);

		v0.normal += normal;
		v0.tangent += tangent;
		v1.normal += normal;
		v1.tangent += tangent;
		v2.normal += normal;
		v2.tangent += tangent;
	}

	usize lv = len(vertices);
	for (usize i = 0; i < lv; i++)
	{
		vertices[i].normal = normalize(vertices[i].normal);
		vertices[i].tangent = normalize(vertices[i].tangent);
	}
}

Mesh generateMesh(const MeshData& data)
{
	Mesh mesh = {};
	mesh.drawType  = data.drawType;
	mesh.drawCount = (s32)len(data.indices);

	glGenBuffers(1, &mesh.vbo);
	glGenBuffers(1, &mesh.ibo);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	defer(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(Vertex) * len(data.vertices),
	             &data.vertices[0],
	             GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	defer(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             sizeof(u32) * len(data.indices),
	             &data.indices[0],
	             GL_STATIC_DRAW);

	return mesh;
}

void drawMesh(const Mesh& mesh)
{
	glEnableVertexAttribArray((u32)AttribLocation::Position);
	glEnableVertexAttribArray((u32)AttribLocation::TexCoord);
	glEnableVertexAttribArray((u32)AttribLocation::Color);
	glEnableVertexAttribArray((u32)AttribLocation::Normal);
	glEnableVertexAttribArray((u32)AttribLocation::Tangent);
	defer(glDisableVertexAttribArray((u32)AttribLocation::Position));
	defer(glDisableVertexAttribArray((u32)AttribLocation::TexCoord));
	defer(glDisableVertexAttribArray((u32)AttribLocation::Color));
	defer(glDisableVertexAttribArray((u32)AttribLocation::Normal));
	defer(glDisableVertexAttribArray((u32)AttribLocation::Tangent));

	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
	defer(glBindBuffer(GL_ARRAY_BUFFER, 0));
	defer(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	glVertexAttribPointer((u32)AttribLocation::Position,
	                      3,
	                      GL_FLOAT,       // Type
	                      false,          // Normalized?
	                      sizeof(Vertex), // Stride
	                      (const void*)(0));
	glVertexAttribPointer((u32)AttribLocation::TexCoord,
	                      2,
	                      GL_FLOAT,         // Type
	                      false,            // Normalized?
	                      sizeof(Vertex),   // Stride
	                      (const void*)(0 + //
	                                    sizeof(Vector3)));
	glVertexAttribPointer((u32)AttribLocation::Color,
	                      4,
	                      GL_UNSIGNED_BYTE,               // 0-255 => 0-1
	                      true,                           // Normalized?
	                      sizeof(Vertex),                 // Stride
	                      (const void*)(0 +               //
	                                    sizeof(Vector3) + //
	                                    sizeof(Vector2)));
	glVertexAttribPointer((u32)AttribLocation::Normal,
	                      3,
	                      GL_FLOAT,                       // Type
	                      false,                          // Normalized?
	                      sizeof(Vertex),                 // Stride
	                      (const void*)(0 +               //
	                                    sizeof(Vector3) + //
	                                    sizeof(Vector2) + //
	                                    sizeof(Color)));
	glVertexAttribPointer((u32)AttribLocation::Tangent,
	                      3,
	                      GL_FLOAT,                       // Type
	                      false,                          // Normalized?
	                      sizeof(Vertex),                 // Stride
	                      (const void*)(0 +               //
	                                    sizeof(Vector3) + //
	                                    sizeof(Vector2) + //
	                                    sizeof(Color)   + //
	                                    sizeof(Vector3)));

	glDrawElements((s32)mesh.drawType, mesh.drawCount, GL_UNSIGNED_INT, nullptr);
}
} // namespace Dunjun
