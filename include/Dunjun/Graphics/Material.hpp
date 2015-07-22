#ifndef DUNJUN_GRAPHICS_MATERIAL_HPP
#define DUNJUN_GRAPHICS_MATERIAL_HPP

#include <Dunjun/Graphics/Texture.hpp>
#include <Dunjun/Graphics/Color.hpp>

namespace Dunjun
{
// TODO(bill): Make PBR style Material
// TODO(bill): Make POD-er
struct Material
{
	/*
	  What a final material will look like
	    diffuse / albedo
	    - map
	    - color

	    specular
	    - map
	    - color
	    - exponent / smoothness


	    // Other things

	    ambient occlusion
	    - map

	    emissive
	    - map

	    detail
	    - map
	    - color
	 */
	enum Type : u32
	{
		Default,
	};

	Type type = Material::Default;

	Texture* diffuseMap = nullptr;
	Color diffuseColor  = Color::White;

	Color specularColor  = Color::White;
	f32 specularExponent = 80.0f;
};
} // namespace Dunjun

#endif
