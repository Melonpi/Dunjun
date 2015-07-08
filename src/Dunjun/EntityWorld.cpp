#include <Dunjun/EntityWorld.hpp>
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/System/Memory.hpp>

#include <Dunjun/ResourceHolders.hpp>

#include <cstdio>

namespace Dunjun
{
EntityWorld::EntityWorld()
: sceneGraph{defaultAllocator()}
, renderSystem{defaultAllocator(), sceneGraph}
, components{} // Init all to zero
, camera{}
{
	camera.transform.position = {0, 1, 3};
	cameraLookAt(camera, {0, 0, 0});
	camera.projectionType = ProjectionType::Perspective;
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

bool EntityWorld::isAlive(EntityId e) const
{
	return components[e] != Component_None;
}

void EntityWorld::destroy(EntityId e) { components[e] = Component_None; }

void EntityWorld::handleEvent(const Event& event)
{
	// TODO(bill):
}

void EntityWorld::update(Time dt)
{
	// TODO(bill):
}

void EntityWorld::render()
{
	renderSystem.resetAllPointers();
	renderSystem.fbSize        = {854, 480};
	renderSystem.camera        = &camera;
	camera.viewportAspectRatio = renderSystem.fbSize.x / renderSystem.fbSize.y;
	renderSystem.render();

	glViewport(0, 0, renderSystem.fbSize.x, renderSystem.fbSize.y);
	glClearColor(0.5, 0.69, 1.0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	{
		ShaderProgram& shaders = g_shaderHolder.get("texPass");
		shaders.use();

		shaders.setUniform("u_scale", Vector3{1, 1, 1});
		shaders.setUniform("u_tex", 0);
		// Texture::bind(&renderSystem.gbuffer.textures[GBuffer::Diffuse], 0);
		Texture::bind(&renderSystem.outTexture.colorTexture, 0);

		drawMesh(g_meshHolder.get("quad"));

		shaders.stopUsing();
	}
}
} // namespace Dunjun
