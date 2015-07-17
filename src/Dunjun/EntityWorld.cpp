#include <Dunjun/EntityWorld.hpp>
#include <Dunjun/SceneGraph.hpp>
#include <Dunjun/Core/Memory.hpp>

#include <Dunjun/ResourceHolders.hpp>

#include <cstdio>

namespace Dunjun
{
EntityWorld::EntityWorld()
: sceneGraph{defaultAllocator()}
, renderSystem{defaultAllocator(), sceneGraph}
, components{}
, names{}
, camera{}
{
	camera.transform.position = {0, 1, 3};
	cameraLookAt(camera, {0, 0, 0});
	camera.fieldOfView    = Degree{90};
	camera.projectionType = ProjectionType::Perspective;
}

EntityId EntityWorld::addEntity(u32 c)
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

bool EntityWorld::isAlive(EntityId e) const
{
	return components[e] != Component_None;
}

void EntityWorld::removeEntity(EntityId e) { components[e] = Component_None; }

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
