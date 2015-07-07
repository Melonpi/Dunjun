#include <Dunjun/Entity.hpp>

#include <cstdio>

namespace Dunjun
{
void EntityWorld::init()
{
	for (auto& c : components)
		c = Component_None;
}

EntityId EntityWorld::createEntity()
{
	EntityId e;
	for (e = 0; e < MaxEntities; e++)
	{
		if (components[e] == Component_None)
			return e;
	}

	fprintf(stderr, "No more entities available.\n");
	return MaxEntities;
}

bool EntityWorld::alive(EntityId e)
{
	return components[e] != Component_None;
}

void EntityWorld::destroy(EntityId e)
{
	components[e] = Component_None;
}
} // namespace Dunjun
