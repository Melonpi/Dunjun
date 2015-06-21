#include <Dunjun/Graphics/Mesh.hpp>

namespace Dunjun
{
void Mesh::Data::generateNormals()
{
	for (u32 i{0}; i < len(indices); i += 3)
	{
		Vertex& v0{vertices[indices[i + 0]]};
		Vertex& v1{vertices[indices[i + 1]]};
		Vertex& v2{vertices[indices[i + 2]]};

		Vector3 a = v1.position - v0.position;
		Vector3 b = v2.position - v1.position;

		Vector3 normal = normalize(cross(a, b));

		v0.normal += normal;
		v1.normal += normal;
		v2.normal += normal;
	}

	for (usize i{0}; i < len(vertices); i++)
		vertices[i].normal = normalize(vertices[i].normal);
}

Mesh::Mesh()
: data{}
, generated{false}
, vbo{0}
, ibo{0}
, drawType{DrawType::Triangles}
, drawCount{0}
{
}

Mesh::Mesh(const Data& data)
: data{data}
, generated{false}
, vbo{0}
, ibo{0}
, drawType{data.drawType}
, drawCount{(s32)len(data.indices)}
{
	generate();
}

void Mesh::addData(const Data& data_)
{
	data = data_;
	drawType = data.drawType;
	drawCount = len(data.indices);
	generated = false;
}

void Mesh::generate() const
{
	if (generated)
		return;

	if (!vbo)
		glGenBuffers(1, &vbo);
	if (!ibo)
		glGenBuffers(1, &ibo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(Vertex) * len(data.vertices),
	             &data.vertices[0],
	             GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             sizeof(u32) * len(data.indices),
	             &data.indices[0],
	             GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	generated = true;
}

void Mesh::draw() const
{
	if (!generated)
		generate();

	glEnableVertexAttribArray((u32)AtrribLocation::Position);
	glEnableVertexAttribArray((u32)AtrribLocation::TexCoord);
	glEnableVertexAttribArray((u32)AtrribLocation::Color);
	glEnableVertexAttribArray((u32)AtrribLocation::Normal);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glVertexAttribPointer((u32)AtrribLocation::Position,
	                      3,
	                      GL_FLOAT,       // Type
	                      false,          // Normalized?
	                      sizeof(Vertex), // Stride
	                      (const GLvoid*)(0));
	glVertexAttribPointer((u32)AtrribLocation::TexCoord,
	                      2,
	                      GL_FLOAT,                 // Type
	                      false,                    // Normalized?
	                      sizeof(Vertex),           // Stride
	                      (const GLvoid*)(0 +       //
	                                      sizeof(Vector3)));
	glVertexAttribPointer((u32)AtrribLocation::Color,
	                      4,
	                      GL_UNSIGNED_BYTE,                 // 0-255 => 0-1
	                      true,                             // Normalized?
	                      sizeof(Vertex),                   // Stride
	                      (const GLvoid*)(0 +               //
	                                      sizeof(Vector3) + //
	                                      sizeof(Vector2)));
	glVertexAttribPointer((u32)AtrribLocation::Normal,
	                      3,
	                      GL_FLOAT,       // Type
	                      false,          // Normalized?
	                      sizeof(Vertex), // Stride
	                      (const GLvoid*)(0 + sizeof(Vector3) +
	                                      sizeof(Vector2) + sizeof(Color)));

	glDrawElements((GLenum)drawType, (s32)drawCount, GL_UNSIGNED_INT, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray((u32)AtrribLocation::Position);
	glDisableVertexAttribArray((u32)AtrribLocation::TexCoord);
	glDisableVertexAttribArray((u32)AtrribLocation::Color);
	glDisableVertexAttribArray((u32)AtrribLocation::Normal);
}
} // namespace Dunjun
