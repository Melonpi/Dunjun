#ifndef DUNJUN_SCENE_SCENERENDERER_HPP
#define DUNJUN_SCENE_SCENERENDERER_HPP

#include <Dunjun/Graphics/Material.hpp>
#include <Dunjun/Graphics/Camera.hpp>
#include <Dunjun/Graphics/ModelAsset.hpp>
#include <Dunjun/Scene/Lighting.hpp>
#include <Dunjun/Graphics/RenderTexture.hpp>
#include <Dunjun/Graphics/GBuffer.hpp>
#include <Dunjun/Window.hpp>

#include <Dunjun/System/Containers.hpp>

namespace Dunjun
{
class SceneNode;
class MeshRenderer;
class World;

class SceneRenderer : private NonCopyable
{
public:
	struct ModelInstance
	{
		const MeshRenderer* meshRenderer = nullptr;
		Transform transform;
	};

	World* world = nullptr;

	u32 fbWidth = 512;
	u32 fbHeight = 512;

	GBuffer gBuffer;
	RenderTexture lBuffer;

	RenderTexture outTexture;

	Color ambientColor = Color{222, 227, 234, 255};
	f32   ambientIntensity = 0.02f;

	const Camera* camera = nullptr;
	const Material* currentMaterial = nullptr;
	const ShaderProgram* currentShaders = nullptr;
	const Texture* currentTexture = nullptr;

	Array<ModelInstance> modelInstances;

	SceneRenderer();
	~SceneRenderer();

	SceneRenderer& reset(); // Reset all pointers
	SceneRenderer& clearAll(); // clear all containers

	SceneRenderer& addSceneGraph(const SceneNode& node, Transform t = Transform{});
	void draw(const Mesh* mesh) const;

	void addModelInstance(const MeshRenderer& meshRenderer, Transform t);

	void render();

	SceneRenderer& geometryPass();
	SceneRenderer& lightPass();
	SceneRenderer& outPass();

private:
	bool setShaders(const ShaderProgram* shaders);
	bool setTexture(const Texture* texture, u32 position);

};
} // namespace Dunjun

#endif
