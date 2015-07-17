#ifndef DUNJUN_ENTITY_HPP
#define DUNJUN_ENTITY_HPP

#include <Dunjun/Types.hpp>

namespace Dunjun
{
using EntityId = u32;

enum ComponentMasks : u32
{
	Component_None      = 0x00000000,
	Component_Name      = 0x00000001,
	Component_Transform = 0x00000002,
	Component_Render    = 0x00000004 | Component_Transform,
};

class EntitySystem
{
public:
	GLOBAL const u32 MaxEntities = 1024;

	u32 components[MaxEntities];

	EntitySystem() = default;

	EntityId addEntity(u32 components);
	void removeEntity(EntityId e);

	bool isAlive(EntityId e) const;
};
} // namespace Dunjun

#endif
