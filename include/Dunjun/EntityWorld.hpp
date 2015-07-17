#ifndef DUNJUN_ENTITYWORLD_HPP
#define DUNJUN_ENTITYWORLD_HPP

#include <Dunjun/Types.hpp>
#include <Dunjun/Math/Types.hpp>
#include <Dunjun/Core/String.hpp>
#include <Dunjun/Core/Time.hpp>
#include <Dunjun/Window/Event.hpp>
#include <Dunjun/Entity.hpp>
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/RenderSystem.hpp>
#include <Dunjun/Graphics/Camera.hpp>


namespace Dunjun
{
enum ComponentMasks : u32
{
	Component_None   = 0x00000000,
	Component_Name   = 0x00000001,
	Component_Render = 0x00000002,
};

struct NameComponent
{
	String name;
};

struct EntityWorld
{
	GLOBAL const u32 MaxEntities = 1024;

	u32 components[MaxEntities];

	NameComponent names[MaxEntities];

	SceneGraph sceneGraph;
	Camera camera;
	RenderSystem renderSystem;

	EntityId player;

	EntityWorld();
	~EntityWorld() = default;

	EntityWorld(const EntityWorld&) = delete;
	EntityWorld& operator=(const EntityWorld&) = delete;

	EntityId addEntity(u32 components);
	void removeEntity(EntityId e);
	bool isAlive(EntityId e) const;

	void handleEvent(const Event& event);
	void update(Time dt);
	void render();
};
} // namespace Dunjun

#endif
