#ifndef DUNJUN_GRAPHICS_SHADERPROGRAM_HPP
#define DUNJUN_GRAPHICS_SHADERPROGRAM_HPP

#include <Dunjun/Math/Types.hpp>
#include <Dunjun/Graphics/Transform.hpp>
#include <Dunjun/Graphics/Color.hpp>

#include <Dunjun/Core/ContainerTypes.hpp>
#include <Dunjun/Core/String.hpp>

namespace Dunjun
{
enum class ShaderType
{
	Vertex,
	Fragment,
};

class ShaderProgram
{
public:
	u32 handle;
	b32 isLinked;
	String errorLog;

	ShaderProgram();
	~ShaderProgram();

	b32 attachShaderFromFile(ShaderType type, const String& filename);
	b32 attachShaderFromMemory(ShaderType type, const String& filename);

	void use() const;
	b32 isInUse() const;
	void stopUsing() const;
	void checkInUse() const;

	b32 link();

	void bindAttribLocation(s32 location, const String& name);

	s32 getAttribLocation(const String& name) const;
	s32 getUniformLocation(const String& name) const;

	void setUniform(const String& name, f32 x) const;
	void setUniform(const String& name, f32 x, f32 y) const;
	void setUniform(const String& name, f32 x, f32 y, f32 z) const;
	void setUniform(const String& name, f32 x, f32 y, f32 z, f32 w) const;
	void setUniform(const String& name, u32 x) const;
	void setUniform(const String& name, s32 x) const;
	void setUniform(const String& name, bool x) const;
	void setUniform(const String& name, const Vector2& v) const;
	void setUniform(const String& name, const Vector3& v) const;
	void setUniform(const String& name, const Vector4& v) const;
	void setUniform(const String& name, const Matrix4& m) const;
	void setUniform(const String& name, const Quaternion& t) const;
	void setUniform(const String& name, const Transform& t) const;
	void setUniform(const String& name, const Color& c) const;

private:
	mutable HashMap<s32> m_attribLocations;
	mutable HashMap<s32> m_uniformLocations;

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
};
} // namespace Dunjun

#endif
