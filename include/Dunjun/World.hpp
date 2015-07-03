#ifndef DUNJUN_WORLD_HPP
#define DUNJUN_WORLD_HPP

#include <Dunjun/Common.hpp>
#include <Dunjun/System.hpp>
#include <Dunjun/Window/Event.hpp>
#include <Dunjun/Scene.hpp>
#include <Dunjun/Graphics.hpp>
#include <Dunjun/Context.hpp>
#include <Dunjun/Level/Level.hpp>

#include <vector>

namespace Dunjun
{
class World : private NonCopyable
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

	std::vector<PointLight> pointLights;
	std::vector<DirectionalLight> directionalLights;
	std::vector<SpotLight> spotLights;

	World();
	virtual ~World();

	void init(Context context);

	void update(Time dt);
	void handleEvent(const Event& event);
	void render();
};
} // namespace Dunjun

#endif
