#ifndef DUNJUN_GRAPHICS_TEXTURE_HPP
#define DUNJUN_GRAPHICS_TEXTURE_HPP

#include <Dunjun/Graphics/Image.hpp>
#include <Dunjun/Core/OpenGL.hpp>
#include <Dunjun/Common.hpp>

namespace Dunjun
{
enum class TextureFilter : s32
{
	Linear  = GL_LINEAR,
	Nearest = GL_NEAREST,
};

enum class TextureWrapMode : s32
{
	Clamp          = GL_CLAMP,
	Repeat         = GL_REPEAT,
	ClampToEdge    = GL_CLAMP_TO_EDGE,
	ClampToBorder  = GL_CLAMP_TO_BORDER,
	MirroredRepeat = GL_MIRRORED_REPEAT,
};

struct Texture
{
	u32 handle;
	s32 width;
	s32 height;
};

Texture
loadTextureFromFile(const String& filename,
                    TextureFilter minMagFilter = TextureFilter::Linear,
                    TextureWrapMode wrapMode   = TextureWrapMode::Repeat);
Texture
loadTextureFromImage(const Image& image,
                     TextureFilter minMagFilter = TextureFilter::Linear,
                     TextureWrapMode wrapMode   = TextureWrapMode::Repeat);

void destroyTexture(Texture& t);

void bindTexture(const Texture* t);
} // namespace Dunjun

#endif
