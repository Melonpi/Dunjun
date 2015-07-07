#include <Dunjun/RenderSystem.hpp>

#include <Dunjun/ResourceHolders.hpp>

namespace Dunjun
{

RenderSystem::RenderSystem(Allocator& a, SceneGraph& sg)
: allocator{a}
, data{}
, map{a}
, sceneGraph{sg}
, gbuffer{}
, lbuffer{}
, ambientColor({222, 227, 234, 255})
, ambientIntensity{0.02f}
, camera{nullptr}
, currentTexture{nullptr}
{
}

void RenderSystem::allocate(u32 capacity)
{
	if (capacity <= data.length)
		return;

	const usize bytes = capacity * (sizeof(EntityId) + sizeof(RenderComponent));

	InstanceData newData;
	newData.length   = data.length;
	newData.capacity = capacity;
	newData.buffer   = allocator.allocate(bytes);

	newData.entityId  = (EntityId*)(newData.buffer);
	newData.component = (RenderComponent*)(newData.entityId + capacity);

	memcpy(newData.entityId, data.entityId, data.length * sizeof(EntityId));
	memcpy(newData.component,
	       data.component,
	       data.length * sizeof(RenderComponent));
	allocator.deallocate(data.buffer);
	data = newData;
}

RenderSystem::ComponentId RenderSystem::create(EntityId id,
                                               const RenderComponent& component)
{
	if (data.capacity == data.length) // grow
		allocate(2 * data.length + 1);

	const ComponentId last = data.length;

	data.entityId[last]  = id;
	data.component[last] = component;

	set(map, id, last);

	data.length++;

	return last;
}

void RenderSystem::destroy(ComponentId id)
{
	const ComponentId last    = data.length - 1;
	const EntityId entity     = data.entityId[id];
	const EntityId lastEntity = data.entityId[last];

	data.entityId[id]  = data.entityId[last];
	data.component[id] = data.component[last];

	set(map, lastEntity, id);
	set(map, entity, EmptyComponentId);

	data.length--;
}

RenderSystem::ComponentId RenderSystem::getComponentId(EntityId id)
{
	return get(map, id, EmptyComponentId);
}

bool RenderSystem::isValid(ComponentId id) const
{
	return id != EmptyComponentId;
}

void RenderSystem::resetAllPointers()
{
	camera         = nullptr;
	currentTexture = nullptr;
}

void RenderSystem::render()
{
	resetAllPointers();

	gbuffer.create(fbSize.x, fbSize.y);

	geometryPass();
}

void RenderSystem::geometryPass()
{
	auto& shaders = g_shaderHolder.get("geometryPass");

	GBuffer::bind(&gbuffer);
	{
		glViewport(0, 0, gbuffer.width, gbuffer.height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaders.use();

		if (!camera)
			return;

		shaders.setUniform("u_camera", cameraMatrix(*camera));
		shaders.setUniform("u_cameraPosition", camera->transform.position);

		for (u32 i = 0; i < data.length; i++)
		{
			EntityId& entityId         = data.entityId[i];
			RenderComponent& component = data.component[i];
			shaders.setUniform("u_material.diffuseColor",
			                   component.material.diffuseColor);
			shaders.setUniform("u_material.specularColor",
			                   component.material.specularColor);
			shaders.setUniform("u_material.specularExponent",
			                   component.material.specularExponent);

			shaders.setUniform("u_material.diffuseMap", (int)0);
			setTexture(component.material.diffuseMap, 0);

			// TODO(bill): improve performance - set at render time
			shaders.setUniform(
			    "u_transform",
			    sceneGraph.getWorldTransform(sceneGraph.getNodeId(entityId)));

			drawMesh(component.mesh);
		}
		// for (const auto& inst : modelInstances)
		// {
		// 	if (inst.meshRenderer->material == nullptr)
		// 		continue;

		// 	{
		// 		shaders.setUniform("u_material.diffuseMap", (int)0);
		// 		shaders.setUniform("u_material.diffuseColor",
		// 		                   inst.meshRenderer->material->diffuseColor);
		// 		shaders.setUniform("u_material.specularColor",
		// 		                   inst.meshRenderer->material->specularColor);
		// 		shaders.setUniform(
		// 		    "u_material.specularExponent",
		// 		    inst.meshRenderer->material->specularExponent);
		// 	}
		// 	setTexture(inst.meshRenderer->material->diffuseMap, 0);

		// 	shaders.setUniform("u_transform", inst.transform);

		// 	draw(inst.meshRenderer->mesh);
		// }

		glFlush();
	}
	GBuffer::bind(nullptr);
}

void RenderSystem::lightPass()
{
	// lbuffer.create(gbuffer.width, gbuffer.height, RenderTexture::Lighting);

	// Texture::bind(&gbuffer.textures[GBuffer::Diffuse], 0);
	// Texture::bind(&gbuffer.textures[GBuffer::Specular], 1);
	// Texture::bind(&gbuffer.textures[GBuffer::Normal], 2);
	// Texture::bind(&gbuffer.textures[GBuffer::Depth], 3);

	// RenderTexture::bind(&lbuffer);
	// {
	// 	glClearColor(0, 0, 0, 0);
	// 	glViewport(0, 0, lbuffer.width, lbuffer.height);
	// 	glClear(GL_COLOR_BUFFER_BIT);

	// 	glDepthMask(false);
	// 	glEnable(GL_BLEND);
	// 	glBlendFunc(GL_ONE, GL_ONE);

	// 	// Ambient Light
	// 	{
	// 		auto& shaders = g_shaderHolder.get("ambientLight");

	// 		shaders.use();

	// 		Vector3 intensities;
	// 		intensities.r = ambientColor.r * ambientIntensity;
	// 		intensities.g = ambientColor.g * ambientIntensity;
	// 		intensities.b = ambientColor.b * ambientIntensity;
	// 		intensities /= 255.0f;

	// 		shaders.setUniform("u_light.intensities", intensities);

	// 		draw(&g_meshHolder.get("quad"));

	// 		shaders.stopUsing();
	// 	}
	// 	// Directional Lights
	// 	{
	// 		auto& shaders = g_shaderHolder.get("directionalLight");

	// 		shaders.use();
	// 		shaders.setUniform("u_specular", 1);
	// 		shaders.setUniform("u_normal", 2);

	// 		for (const auto& light : world->directionalLights)
	// 		{
	// 			Vector3 lightIntensities;

	// 			lightIntensities.r = light.color.r / 255.0f;
	// 			lightIntensities.g = light.color.g / 255.0f;
	// 			lightIntensities.b = light.color.b / 255.0f;
	// 			lightIntensities *= light.intensity;

	// 			shaders.setUniform("u_light.base.intensities",
	// 			                   lightIntensities);
	// 			shaders.setUniform("u_light.direction",
	// 			                   normalize(light.direction));

	// 			draw(&g_meshHolder.get("quad"));
	// 		}
	// 		shaders.stopUsing();
	// 	}
	// 	// Point Lights
	// 	{
	// 		auto& shaders = g_shaderHolder.get("pointLight");

	// 		shaders.use();
	// 		shaders.setUniform("u_diffuse", 0);
	// 		shaders.setUniform("u_specular", 1);
	// 		shaders.setUniform("u_normal", 2);
	// 		shaders.setUniform("u_depth", 3);

	// 		shaders.setUniform("u_cameraInverse",
	// 		                   inverse(cameraMatrix(*camera)));

	// 		for (const PointLight& light : world->pointLights)
	// 		{
	// 			light.range = calculateLightRange(light);

	// 			Vector3 lightIntensities;

	// 			lightIntensities.r = light.color.r / 255.0f;
	// 			lightIntensities.g = light.color.g / 255.0f;
	// 			lightIntensities.b = light.color.b / 255.0f;
	// 			lightIntensities *= light.intensity;

	// 			shaders.setUniform("u_light.base.intensities",
	// 			                   lightIntensities);
	// 			shaders.setUniform("u_light.position", light.position);

	// 			shaders.setUniform("u_light.attenuation.constant",
	// 			                   light.attenuation.constant);
	// 			shaders.setUniform("u_light.attenuation.linear",
	// 			                   light.attenuation.linear);
	// 			shaders.setUniform("u_light.attenuation.quadratic",
	// 			                   light.attenuation.quadratic);

	// 			shaders.setUniform("u_light.range", light.range);

	// 			draw(&g_meshHolder.get("quad"));
	// 		}

	// 		shaders.stopUsing();
	// 	}
	// 	// Spot Lights
	// 	{
	// 		auto& shaders = g_shaderHolder.get("spotLight");

	// 		shaders.use();
	// 		shaders.setUniform("u_diffuse", 0);
	// 		shaders.setUniform("u_specular", 1);
	// 		shaders.setUniform("u_normal", 2);
	// 		shaders.setUniform("u_depth", 3);

	// 		shaders.setUniform("u_cameraInverse",
	// 		                   inverse(cameraMatrix(*camera)));

	// 		for (const SpotLight& light : world->spotLights)
	// 		{
	// 			light.range = calculateLightRange(light);

	// 			Vector3 lightIntensities;

	// 			lightIntensities.r = light.color.r / 255.0f;
	// 			lightIntensities.g = light.color.g / 255.0f;
	// 			lightIntensities.b = light.color.b / 255.0f;
	// 			lightIntensities *= light.intensity;

	// 			shaders.setUniform("u_light.point.base.intensities",
	// 			                   lightIntensities);
	// 			shaders.setUniform("u_light.point.position", light.position);

	// 			shaders.setUniform("u_light.point.attenuation.constant",
	// 			                   light.attenuation.constant);
	// 			shaders.setUniform("u_light.point.attenuation.linear",
	// 			                   light.attenuation.linear);
	// 			shaders.setUniform("u_light.point.attenuation.quadratic",
	// 			                   light.attenuation.quadratic);

	// 			shaders.setUniform("u_light.point.range", light.range);

	// 			shaders.setUniform("u_light.direction", light.direction);
	// 			shaders.setUniform("u_light.coneAngle",
	// 			                   static_cast<f32>(light.coneAngle));

	// 			draw(&g_meshHolder.get("quad"));
	// 		}

	// 		shaders.stopUsing();
	// 	}

	// 	glDisable(GL_BLEND);
	// 	glDepthMask(true);
	// }
	// RenderTexture::bind(nullptr);
}

void RenderSystem::outPass()
{
	// outTexture.create(gbuffer.width, gbuffer.height, RenderTexture::Color);

	// Texture::bind(&gbuffer.textures[GBuffer::Diffuse], 0);
	// Texture::bind(&lbuffer.colorTexture, 1);

	// RenderTexture::bind(&outTexture);
	// {
	// 	glClearColor(1, 1, 1, 1);
	// 	glViewport(0, 0, outTexture.width, outTexture.height);
	// 	glClear(GL_COLOR_BUFFER_BIT);

	// 	auto& shaders = g_shaderHolder.get("out");

	// 	shaders.use();
	// 	shaders.setUniform("u_diffuse", 0);
	// 	shaders.setUniform("u_lighting", 1);

	// 	draw(&g_meshHolder.get("quad"));

	// 	shaders.stopUsing();
	// }
	// RenderTexture::bind(nullptr);
}

bool RenderSystem::setTexture(const Texture* texture, u32 position)
{
	if (texture != currentTexture)
	{
		currentTexture = texture;

		Texture::bind(currentTexture, position);

		return true;
	}

	return false;
}
} // namespace Dunjun
