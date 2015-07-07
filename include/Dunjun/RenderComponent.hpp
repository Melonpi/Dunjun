#ifndef DUNJUN_RENDERCOMPONENT_HPP
#define DUNJUN_RENDERCOMPONENT_HPP

#include <Dunjun/Graphics/Mesh.hpp>
#include <Dunjun/Graphics/Material.hpp>

namespace Dunjun
{
struct RenderComponent
{
	Mesh mesh;
	Material material;
};
} // namespace Dunjun

#endif
