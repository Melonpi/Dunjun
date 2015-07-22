#include <Dunjun/Graphics/ShaderProgram.hpp>

#include <Dunjun/Core/OpenGL.hpp>
#include <Dunjun/Core/Memory.hpp>

#include <Dunjun/Core/Memory.hpp>
#include <Dunjun/Core/Array.hpp>
#include <Dunjun/Core/HashMap.hpp>
#include <Dunjun/Core/FileSystem.hpp>

#include <Dunjun/Core/StringFunctions.hpp>
#include <Dunjun/Core/Murmur.hpp>

#include <fstream>

#include <cstdlib>

namespace Dunjun
{
INTERNAL String shaderSourceFromFile(const String& filename)
{
	String filePath = BaseDirectory::Shaders + filename;

	std::ifstream file;
	file.open(cString(filePath), std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		panic("Failed to open shader file: " + filePath);
		return {};
	}
	defer(file.close());

	String output;

	String line;

	while (file.good())
	{
		getline(file, line);
		line = Strings::trimSpace(line);

		if (Strings::hasPrefix(line, "#include"))
		{
			String includeFilename = substring(line, 8, len(line));
			includeFilename = Strings::trimSpace(includeFilename);
			includeFilename = Strings::trim(includeFilename, "\"");

			if (len(includeFilename) > 0)
			{
				// Recursively append source of header file and append header
				// extension
				const String& withExt = cString(includeFilename + ".head.glsl");
				append(output, shaderSourceFromFile(withExt));
			}
		}
		else
		{
			append(output, line);
		}
		append(output, '\n'); // Append a *nix newline
	}

	return output;
}

ShaderProgram::ShaderProgram()
: handle{0}
, isLinked{false}
, errorLog{}

, m_attribLocations{defaultAllocator()}
, m_uniformLocations{defaultAllocator()}

{
}

ShaderProgram::~ShaderProgram()
{
	if (handle)
		glDeleteProgram(handle);
}

b32 ShaderProgram::attachShaderFromFile(ShaderType type,
                                        const String& filename)
{
	const String source = shaderSourceFromFile(filename);
	return attachShaderFromMemory(type, source);
}

b32 ShaderProgram::attachShaderFromMemory(ShaderType type,
                                          const String& shaderSource)
{
	if (!handle)
		handle = glCreateProgram();

	u32 shader = 0;
	if (type == ShaderType::Vertex)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (type == ShaderType::Fragment)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else
		panic("Unknown shader type.");

	const char* cStrSource = cString(shaderSource);
	glShaderSource(shader, 1, &cStrSource, nullptr);
	glCompileShader(shader);

	s32 status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == false)
	{
		String msg;
		if (type == ShaderType::Vertex)
			msg = "Compile failure in vertex shader: \n";
		else if (type == ShaderType::Fragment)
			msg = "Compile failure in fragment shader: \n";
		else
			panic("Unknown shader type.");

		s32 infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		Allocator& a = defaultAllocator();

		char* strInfoLog =
		    (char*)a.allocate((infoLogLength + 1) * sizeof(char));
		defer(a.deallocate(strInfoLog));

		glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog);

		append(msg, strInfoLog);
		append(msg, '\n');

		append(errorLog, msg);

		glDeleteShader(shader);

		return false;
	}

	glAttachShader(handle, shader);

	return true;
}

void ShaderProgram::use() const
{
	if (!isInUse())
		glUseProgram(handle);
}

b32 ShaderProgram::isInUse() const
{
	s32 currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

	return (currentProgram == (s32)handle);
}

void ShaderProgram::stopUsing() const
{
	if (isInUse())
		glUseProgram(0);
}

void ShaderProgram::checkInUse() const
{
	if (!isInUse())
		panic("ShaderProgram not in use.");
}

b32 ShaderProgram::link()
{
	if (!handle)
		handle = glCreateProgram();

	if (!isLinked)
	{
		glLinkProgram(handle);

		s32 status;
		glGetProgramiv(handle, GL_LINK_STATUS, &status);
		if (!status)
		{
			String msg{"ShaderProgram linking failure: \n"};

			s32 infoLogLength;
			glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &infoLogLength);

			Allocator& a = defaultAllocator();

			char* strInfoLog =
			    (char*)a.allocate((infoLogLength + 1) * sizeof(char));
			defer(a.deallocate(strInfoLog));

			glGetProgramInfoLog(handle, infoLogLength, nullptr, strInfoLog);

			append(msg, strInfoLog);
			append(msg, "\n");
			append(errorLog, msg);

			glDeleteProgram(handle);
			handle = 0;
			isLinked = false;

			return isLinked;
		}

		isLinked = true;
	}

	return isLinked;
}

void ShaderProgram::bindAttribLocation(s32 location, const String& name)
{
	glBindAttribLocation(handle, location, cString(name));
	set(m_attribLocations, stringHash(name), location);
}

s32 ShaderProgram::getAttribLocation(const String& name) const
{
	const u64 hash = stringHash(name);

	if (has(m_attribLocations, hash))
		return get(m_attribLocations, hash, 0);

	s32 loc = glGetAttribLocation(handle, cString(name));
	set(m_attribLocations, hash, loc);
	return loc;
}

s32 ShaderProgram::getUniformLocation(const String& name) const
{
	const u64 hash = stringHash(name);

	if (has(m_uniformLocations, hash))
		return get(m_uniformLocations, hash, 0);

	s32 loc = glGetUniformLocation(handle, cString(name));
	set(m_uniformLocations, hash, loc);
	return loc;
}

void ShaderProgram::setUniform(const String& name, f32 x) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform1f(loc, x);
}

void ShaderProgram::setUniform(const String& name, f32 x, f32 y) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform2f(loc, x, y);
}

void ShaderProgram::setUniform(const String& name, f32 x, f32 y, f32 z) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform3f(loc, x, y, z);
}

void ShaderProgram::setUniform(
    const String& name, f32 x, f32 y, f32 z, f32 w) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform4f(loc, x, y, z, w);
}

void ShaderProgram::setUniform(const String& name, u32 x) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform1ui(loc, x);
}

void ShaderProgram::setUniform(const String& name, s32 x) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform1i(loc, x);
}

void ShaderProgram::setUniform(const String& name, bool x) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform1i(loc, (s32)x);
}

void ShaderProgram::setUniform(const String& name, const Vector2& v) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform2fv(loc, 1, &v[0]);
}

void ShaderProgram::setUniform(const String& name, const Vector3& v) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform3fv(loc, 1, &v[0]);
}

void ShaderProgram::setUniform(const String& name, const Vector4& v) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform4fv(loc, 1, &v[0]);
}

void ShaderProgram::setUniform(const String& name, const Matrix4& m) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniformMatrix4fv(loc, 1, false, &m[0][0]);
}

void ShaderProgram::setUniform(const String& name, const Quaternion& q) const
{
	checkInUse();
	s32 loc = getUniformLocation(name);
	if (loc == -1)
		return;
	glUniform4fv(loc, 1, &q.x);
}

void ShaderProgram::setUniform(const String& name, const Transform& t) const
{
	checkInUse();
	{
		s32 loc = getUniformLocation(name + ".position");
		if (loc == -1)
			return;
		glUniform3fv(loc, 1, &t.position[0]);
	}
	{
		s32 loc = getUniformLocation(name + ".orientation");
		if (loc == -1)
			return;
		glUniform4fv(loc, 1, &t.orientation.x);
	}
	{
		s32 loc = getUniformLocation(name + ".scale");
		if (loc == -1)
			return;
		glUniform3fv(loc, 1, &t.scale[0]);
	}
}

void ShaderProgram::setUniform(const String& name, const Color& c) const
{
	f32 r = c.r / 255.0f;
	f32 g = c.g / 255.0f;
	f32 b = c.b / 255.0f;
	f32 a = c.a / 255.0f;

	setUniform(name, r, g, b, a);
}
} // namespace Dunjun
