#ifndef DUNJUN_GRAPHICS_TEXTURE_HPP
#define DUNJUN_GRAPHICS_TEXTURE_HPP

#include <Dunjun/System/NonCopyable.hpp>
#include <Dunjun/Graphics/Image.hpp>
#include <Dunjun/System/OpenGL.hpp>
#include <Dunjun/Common.hpp>

namespace Dunjun
{
enum class TextureFilter : s32
{
	Linear = GL_LINEAR,
	Nearest = GL_NEAREST,
};

enum class TextureWrapMode : s32
{
	Clamp = GL_CLAMP,
	Repeat = GL_REPEAT,
	ClampToEdge = GL_CLAMP_TO_EDGE,
	ClampToBorder = GL_CLAMP_TO_BORDER,
	MirroredRepeat = GL_MIRRORED_REPEAT,
};

struct Texture : private NonCopyable
{
	u32 handle{0};
	s32 width{0};
	s32 height{0};

	Texture() = default;
	Texture(const Image& image,
	        TextureFilter minMagFilter = TextureFilter::Linear,
	        TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

	bool loadFromFile(const std::string& filename,
	                  TextureFilter minMagFilter = TextureFilter::Linear,
	                  TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);
	bool loadFromImage(const Image& image,
	                   TextureFilter minMagFilter = TextureFilter::Linear,
	                   TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

	virtual ~Texture();

	GLOBAL void bind(const Texture* tex, u32 position);
};
} // namespace Dunjun

#endif
