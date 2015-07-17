#ifndef DUNJUN_GRAPHICS_GBUFFER_HPP
#define DUNJUN_GRAPHICS_GBUFFER_HPP

#include <Dunjun/Core/OpenGL.hpp>
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

	u32 width  = 0;
	u32 height = 0;
	u32 fbo    = 0;
	Texture textures[TextureType::Count];
};

bool createGBuffer(GBuffer& b, u32 width, u32 height);
void destroyGBuffer(GBuffer& b);
void bindGBuffer(const GBuffer* b);

} // namespace Dunjun

#endif
