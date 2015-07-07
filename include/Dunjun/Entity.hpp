#ifndef DUNJUN_ENTITY_HPP
#define DUNJUN_ENTITY_HPP

#include <Dunjun/Types.hpp>
#include <Dunjun/Math/Types.hpp>

#include <string>

namespace Dunjun
{
using EntityId = u32;

enum ComponentMasks : u32
{
	Component_None     = 0x00000000,
	Component_Position = 0x00000001,
	Component_Name     = 0x00000002,
};

struct PositionComponent
{
	Vector3 position;
};

struct NameComponent
{
	std::string name;
};

#define MaxEntities 1024

struct EntityWorld
{
	u32 components[MaxEntities];

	// TODO(bill): Use Custom Systems
	PositionComponent positions[MaxEntities];
	NameComponent names[MaxEntities];

	void init();

	EntityId createEntity();
	bool alive(EntityId e);
	void destroy(EntityId e);
};
} // namespace Dunjun

#endif
