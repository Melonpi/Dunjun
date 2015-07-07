#ifndef DUNJUN_ENTITYWORLD_HPP
#define DUNJUN_ENTITYWORLD_HPP

#include <Dunjun/Types.hpp>
#include <Dunjun/Math/Types.hpp>
#include <Dunjun/Entity.hpp>
#include <Dunjun/SceneGraph.hpp>

#include <string>

namespace Dunjun
{
enum ComponentMasks : u32
{
	Component_None = 0x00000000,
	Component_Name = 0x00000001,
};

struct NameComponent
{
	std::string name;
};

#define MaxEntities 1024

struct EntityWorld
{
	u32 components[MaxEntities];

	NameComponent names[MaxEntities];

	SceneGraph sceneGraph;

	EntityWorld();
	~EntityWorld() = default;

	EntityId createEntity();
	bool alive(EntityId e);
	void destroy(EntityId e);
};
} // namespace Dunjun

#endif
