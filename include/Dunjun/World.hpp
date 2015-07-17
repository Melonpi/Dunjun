#ifndef DUNJUN_WORLD_HPP
#define DUNJUN_WORLD_HPP

#include <Dunjun/Common.hpp>
#include <Dunjun/System.hpp>
#include <Dunjun/Window/Event.hpp>
#include <Dunjun/Scene.hpp>
#include <Dunjun/Graphics.hpp>
#include <Dunjun/Context.hpp>
#include <Dunjun/Level/Level.hpp>

#include <Dunjun/Core/ContainerTypes.hpp>

namespace Dunjun
{
class World
{
public:
	Context context;

	SceneNode sceneGraph;
	SceneRenderer renderer;

	Camera mainCamera;
	Camera playerCamera;
	Camera* currentCamera = &playerCamera;

	SceneNode* player = nullptr;
	Level* level      = nullptr;

	Array<PointLight> pointLights;
	Array<DirectionalLight> directionalLights;
	Array<SpotLight> spotLights;

	World();
	virtual ~World();

	void init(Context context);

	void update(Time dt);
	void handleEvent(const Event& event);
	void render();

private:
	World(const World&) = delete;
	World& operator=(const World&) = delete;
};
} // namespace Dunjun

#endif
