#ifndef DUNJUN_GRAPHICS_GBUFFER_HPP
#define DUNJUN_GRAPHICS_GBUFFER_HPP

#include <Dunjun/System/OpenGL.hpp>
#include <Dunjun/Types.hpp>
#include <Dunjun/Graphics/Texture.hpp>

namespace Dunjun
{
struct GBuffer
{
	enum TextureType
	{
		Diffuse, // TODO(bill): Rename to albedo? (PBR Shaders...)
		Specular,
		Normal,
		Depth,
		// TODO(bill): Emission,
		Count,
	};

	u32 width;
	u32 height;
	u32 fbo;
	Texture textures[TextureType::Count];

	bool create(u32 width, u32 height);
	void destroy();

	GLOBAL void bind(const GBuffer* b);
};
} // namespace Dunjun

#endif
