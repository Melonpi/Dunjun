#ifndef DUNJUN_WORLD_HPP
#define DUNJUN_WORLD_HPP

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
class World
{
public:
	EntitySystem entitySystem;

	String names[EntitySystem::MaxEntities];
	SceneGraph sceneGraph;
	RenderSystem renderSystem;

	Camera camera;

	EntityId player;

	World();
	~World() = default;

private:
	World(const World&) = delete;
	World& operator=(const World&) = delete;
};
} // namespace Dunjun

#endif
