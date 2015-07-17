#include <Dunjun/World.hpp>
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/Core/Memory.hpp>

#include <Dunjun/ResourceHolders.hpp>

#include <cstdio>

namespace Dunjun
{
World::World()
: entitySystem{}
, sceneGraph{defaultAllocator()}
, renderSystem{defaultAllocator(), sceneGraph}
, names{}
, camera{}
{
	camera.transform.position = {0, 1, 3};
	cameraLookAt(camera, {0, 0, 0});
	camera.fieldOfView    = Degree{90};
	camera.projectionType = ProjectionType::Perspective;
}

void World::handleEvent(const Event& event)
{
	// TODO(bill):
}

void World::update(Time dt)
{
	// TODO(bill):
}

void World::render()
{
	renderSystem.resetAllPointers();
	renderSystem.camera        = &camera;
	camera.viewportAspectRatio = renderSystem.fbSize.x / renderSystem.fbSize.y;
	renderSystem.render();

	{
		ShaderProgram& shaders = g_shaderHolder.get("texPass");
		shaders.use();
		defer(shaders.stopUsing());

		shaders.setUniform("u_scale", Vector3{1, 1, 1});
		shaders.setUniform("u_tex", 0);
		// bindTexture(&renderSystem.gbuffer.textures[GBuffer::Diffuse], 0);
		bindTexture(&renderSystem.outTexture.colorTexture, 0);

		drawMesh(g_meshHolder.get("quad"));
	}
}
} // namespace Dunjun
