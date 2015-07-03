#ifndef DUNJUN_GRAPHICS_GBUFFER_HPP
#define DUNJUN_GRAPHICS_GBUFFER_HPP

#include <Dunjun/System/OpenGL.hpp>
#include <Dunjun/Types.hpp>
#include <Dunjun/Graphics/Texture.hpp>

namespace Dunjun
{
struct GBuffer
{
public:
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

	virtual ~GBuffer();

	bool create(u32 width, u32 height);

	GLOBAL void bind(const GBuffer* b);
};
} // namespace Dunjun

#endif
