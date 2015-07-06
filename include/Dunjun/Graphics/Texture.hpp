#ifndef DUNJUN_GRAPHICS_TEXTURE_HPP
#define DUNJUN_GRAPHICS_TEXTURE_HPP

#include <Dunjun/Graphics/Image.hpp>
#include <Dunjun/System/OpenGL.hpp>
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

	GLOBAL void bind(const Texture* tex, u32 position);
};

Texture loadTextureFromFile(
    const char* filename,
    TextureFilter minMagFilter = TextureFilter::Linear,
    TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);
Texture loadTextureFromImage(
    const Image& image,
    TextureFilter minMagFilter = TextureFilter::Linear,
    TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

void destroyTexture(Texture& t);
} // namespace Dunjun

#endif
