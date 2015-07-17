#ifndef DUNJUN_GRAPHICS_RENDERTEXTURE_HPP
#define DUNJUN_GRAPHICS_RENDERTEXTURE_HPP

#include <Dunjun/Core/OpenGL.hpp>
#include <Dunjun/Types.hpp>
#include <Dunjun/Graphics/Texture.hpp>

namespace Dunjun
{
struct RenderTexture
{
	enum TextureType
	{
		Color         = 1,
		Depth         = 2,
		ColorAndDepth = Color | Depth,
		Lighting      = 4 | Color,
	};

	Texture colorTexture = {};
	Texture depthTexture = {};

	u32 width  = 0;
	u32 height = 0;
	u32 fbo    = 0;

	TextureType type = TextureType::Color;
};

bool createRenderTexture(
    RenderTexture& rt,
    u32 width,
    u32 height,
    RenderTexture::TextureType type = RenderTexture::ColorAndDepth,
    TextureFilter minMagFilter      = TextureFilter::Linear,
    TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

void destroyRenderTexture(RenderTexture& rt);

void bindRenderTexture(const RenderTexture* rt);
} // namespace Dunjun

#endif
