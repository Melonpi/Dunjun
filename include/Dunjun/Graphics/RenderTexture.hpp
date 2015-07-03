#ifndef DUNJUN_GRAPHICS_RENDERTEXTURE_HPP
#define DUNJUN_GRAPHICS_RENDERTEXTURE_HPP

#include <Dunjun/System/OpenGL.hpp>
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

	Texture colorTexture{};
	Texture depthTexture{};

	u32 width  = 0;
	u32 height = 0;

	TextureType type = TextureType::Color;
	u32 fbo          = 0;

	RenderTexture() = default;
	virtual ~RenderTexture();

	bool create(u32 width,
	            u32 height,
	            TextureType type           = ColorAndDepth,
	            TextureFilter minMagFilter = TextureFilter::Linear,
	            TextureWrapMode wrapMode = TextureWrapMode::ClampToEdge);

	GLOBAL void bind(const RenderTexture* rt);
};
} // namespace Dunjun

#endif
