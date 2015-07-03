#ifndef DUNJUN_CONTEXT_HPP
#define DUNJUN_CONTEXT_HPP

#include <Dunjun/Window/Window.hpp>
#include <Dunjun/ResourceHolders.hpp>

namespace Dunjun
{
struct Context
{
	Window* window;
	ShaderHolder* shaderHolder;
	MeshHolder* meshHolder;
	MaterialHolder* materialHolder;

	Context() = default;

	Context(Window& window,
	        ShaderHolder& shaderHolder,
	        MeshHolder& meshHolder,
	        MaterialHolder& materialHolder)
	: window{&window}
	, shaderHolder{&shaderHolder}
	, meshHolder{&meshHolder}
	, materialHolder{&materialHolder}
	{
	}
};
} // namespace Dunjun

#endif
