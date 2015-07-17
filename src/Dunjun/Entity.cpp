#include <Dunjun/Entity.hpp>

#include <cstdio>

namespace Dunjun
{
EntityId EntitySystem::addEntity(u32 c)
{
	EntityId e;
	for (e = 0; e < MaxEntities; e++)
	{
		if (components[e] == Component_None)
		{
			components[e] = c;
			return e;
		}
	}

	fprintf(stderr, "No more entities available.\n");
	return MaxEntities;
}

void EntitySystem::removeEntity(EntityId e)
{
	components[e] = Component_None;
}

bool EntitySystem::isAlive(EntityId e) const
{
	return components[e] != Component_None;
}
} // namespace Dunjun
